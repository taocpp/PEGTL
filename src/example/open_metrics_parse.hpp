// Copyright (c) 2026 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_SRC_EXAMPLE_OPEN_METRICS_PARSE_HPP
#define TAO_PEGTL_SRC_EXAMPLE_OPEN_METRICS_PARSE_HPP

#include <filesystem>
#include <optional>
#include <stdexcept>
#include <string>
#include <string_view>
#include <utility>

#include <tao/pegtl.hpp>

#include <tao/pegtl/action/builders.hpp>
#include <tao/pegtl/example/open_metrics.hpp>
#include <tao/pegtl/extra/builders.hpp>

#include "open_metrics_model.hpp"

namespace open_metrics
{
   namespace pegtl = tao::pegtl;
   namespace rules = tao::pegtl::open_metrics;

   // This example favors PEGTL value construction over complete OpenMetrics
   // validation. The separate check function reports all retained model-level
   // issues after construction. Compared to the OpenMetrics 1.0 specification
   //
   // parsing and construction accept MORE in the following ways:
   //
   // - MetricFamilies, Metrics, MetricPoints, UNIT/HELP metadata, and samples may
   //   be interleaved.
   // - Numeric semantic restrictions are not rejected. This includes finiteness,
   //   signs, integer-valued counts and buckets, quantile ranges, and monotonicity.
   // - Required, unique, and ordered components are not rejected. This includes
   //   the +Inf bucket, bucket source ordering, bucket and quantile uniqueness,
   //   and duplicate sample components, which can overwrite previous values.
   // - Unit suffixes, label relationships, sample suffix/type compatibility, and
   //   the required StateSet and Info sample values are not consistently rejected.
   //
   // it accepts or retains LESS in the following ways:
   //
   // - When present, TYPE must be the first item seen for a MetricFamily, whereas
   //   OpenMetrics only recommends TYPE, UNIT, HELP order within the metadata.
   // - All numbers are stored as double, so sufficiently large integers and
   //   high-precision timestamps lose precision or can fail conversion.
   // - Info labels are all stored as MetricPoint data labels; the distinction
   //   between Metric labels and Info data labels is not reconstructed.
   // - Exemplars are parsed by the grammar but discarded, as permitted for an
   //   ingestor by the specification.

   [[nodiscard]] inline double parse_label_number( const std::string_view view )
   {
      std::size_t pos = 0;
      const double result = std::stod( std::string( view ), &pos );

      if( pos != view.size() ) {
         throw std::runtime_error( "trailing garbage after number" );
      }
      return result;
   }

   template< typename Data >
   [[nodiscard]] auto& find_metric( Data& data, const label_set& labels )
   {
      for( auto& metric : data.metrics ) {
         if( metric.labels == labels ) {
            return metric;
         }
      }
      return data.metrics.emplace_back( labels );
   }

   template< typename Metric >
   [[nodiscard]] auto& find_point( Metric& metric, const std::optional< double >& timestamp )
   {
      for( auto& point : metric.metric_points ) {
         if( point.timestamp == timestamp ) {
            return point;
         }
      }
      return metric.metric_points.emplace_back( timestamp );
   }

   enum class sample_component
   {
      value,
      count,
      sum,
      bucket,
      created
   };

   struct suffix_mapping
   {
      std::string_view suffix;
      sample_component component;
   };

   struct sample_state
   {
      std::string name;
      std::string family_name;
      label_set labels;
      sample_component component = sample_component::value;
      double value = 0.0;
      std::optional< double > timestamp;
   };

   struct builder
   {
      [[nodiscard]] metric_set&& get() && noexcept
      {
         return std::move( metrics );
      }

      void type_descriptor( std::string&& name, const metric_type type )
      {
         if( !metrics.metric_families.emplace( name, make_family( name, type ) ).second ) {
            throw std::runtime_error( "TYPE must be the first item for metric family " + name );
         }
      }

      void unit_descriptor( std::string&& name, std::string&& value )
      {
         descriptor_string( name, std::move( value ), "unit", &metric_family::unit );
      }

      void help_descriptor( std::string&& name, std::string&& value )
      {
         descriptor_string( name, std::move( value ), "help", &metric_family::help );
      }

      void sample( std::string&& name, std::optional< label_set >&& labels, const double value, const std::optional< double >& timestamp )
      {
         sample_state sample{
            .name = std::move( name ),
            .family_name{},
            .labels = std::move( labels ).value_or( label_set() ),
            .value = value,
            .timestamp = timestamp
         };
         auto& family = resolve_sample_name( sample );
         std::visit( [ & ]( auto& data ) { add_sample( data, sample ); }, family.data );
      }

   private:
      metric_set metrics;

      [[nodiscard]] static metric_family make_family( const std::string& name, const metric_type type )
      {
         switch( type ) {
            case metric_type::unknown:
               return metric_family( name, unknown_family_data() );
            case metric_type::gauge:
               return metric_family( name, gauge_family_data() );
            case metric_type::counter:
               return metric_family( name, counter_family_data() );
            case metric_type::stateset:
               return metric_family( name, state_set_family_data() );
            case metric_type::info:
               return metric_family( name, info_family_data() );
            case metric_type::histogram:
               return metric_family( name, histogram_family_data() );
            case metric_type::gaugehistogram:
               return metric_family( name, gauge_histogram_family_data() );
            case metric_type::summary:
               return metric_family( name, summary_family_data() );
         }
         throw std::runtime_error( "invalid metric family type" );
      }

      [[nodiscard]] metric_family& ensure_family( const std::string& name, const metric_type type )
      {
         auto [ i, inserted ] = metrics.metric_families.emplace( name, make_family( name, type ) );
         if( inserted ) {
            return i->second;
         }

         const auto current = type_of( i->second );
         if( current != type ) {
            throw std::runtime_error( "conflicting type for metric family " + name );
         }
         return i->second;
      }

      void descriptor_string( const std::string& name,
                              std::string&& value,
                              const char* kind,
                              std::optional< std::string > metric_family::* field )
      {
         auto i = metrics.metric_families.find( name );
         if( i == metrics.metric_families.end() ) {
            i = metrics.metric_families.emplace( name, make_family( name, metric_type::unknown ) ).first;
         }
         auto& family = i->second;
         auto& target = family.*field;
         if( target ) {
            throw std::runtime_error( std::string( "duplicate " ) + kind + " for metric family " + name );
         }
         target.emplace( std::move( value ) );
      }

      [[nodiscard]] metric_family& resolve_sample_name( sample_state& sample )
      {
         if( const auto i = metrics.metric_families.find( sample.name ); i != metrics.metric_families.end() ) {
            set_sample_family( sample, sample.name, sample_component::value );
            return i->second;
         }

         static constexpr suffix_mapping suffixes[] = {
            { "_total",   sample_component::value   },
            { "_gcount",  sample_component::count   },
            { "_gsum",    sample_component::sum     },
            { "_info",    sample_component::value   },
            { "_bucket",  sample_component::bucket  },
            { "_count",   sample_component::count   },
            { "_sum",     sample_component::sum     },
            { "_created", sample_component::created }
         };

         for( const auto& suffix : suffixes ) {
            const std::string_view name = sample.name;
            if( name.ends_with( suffix.suffix ) ) {
               std::string family_name( name.substr( 0, name.size() - suffix.suffix.size() ) );
               const auto i = metrics.metric_families.find( family_name );
               if( ( i != metrics.metric_families.end() ) && ( type_of( i->second ) != metric_type::unknown ) ) {
                  set_sample_family( sample, std::move( family_name ), suffix.component );
                  return i->second;
               }
            }
         }
         set_sample_family( sample, sample.name, sample_component::value );
         return ensure_family( sample.family_name, metric_type::unknown );
      }

      static void set_sample_family( sample_state& sample, std::string family_name, const sample_component component )
      {
         sample.family_name = std::move( family_name );
         sample.component = component;
      }

      [[nodiscard]] static label_set labels_without( const sample_state& sample, const std::string& name )
      {
         label_set labels = sample.labels;
         labels.erase( name );
         return labels;
      }

      template< typename Data >
      [[nodiscard]] auto& point_for( Data& data, const sample_state& sample, const label_set& labels )
      {
         return find_point( find_metric( data, labels ), sample.timestamp );
      }

      template< typename Data >
      [[nodiscard]] auto& point_for( Data& data, const sample_state& sample )
      {
         return point_for( data, sample, sample.labels );
      }

      template< typename Value >
      void add_sample( metric_family_data< Value >& data, const sample_state& sample )
      {
         point_for( data, sample ).value.value = sample.value;
      }

      template< typename Data >
      void add_bucket_sample( Data& data, const sample_state& sample )
      {
         const auto label = sample.labels.find( "le" );
         if( label == sample.labels.end() ) {
            throw std::runtime_error( "bucket without le label" );
         }
         const double upper_bound = parse_label_number( label->second );
         point_for( data, sample, labels_without( sample, "le" ) ).value.buckets[ upper_bound ] = sample.value;
      }

      void add_sample( counter_family_data& data, const sample_state& sample )
      {
         auto& point = point_for( data, sample );

         switch( sample.component ) {
            case sample_component::value:
               point.value.total = sample.value;
               break;
            case sample_component::created:
               point.value.created = sample.value;
               break;
            default:
               throw std::runtime_error( "invalid counter sample " + sample.name );
         }
      }

      void add_sample( state_set_family_data& data, const sample_state& sample )
      {
         const auto state_label = sample.labels.find( sample.family_name );
         if( state_label == sample.labels.end() ) {
            throw std::runtime_error( "stateset sample without state label " + sample.family_name );
         }

         auto& point = point_for( data, sample, labels_without( sample, sample.family_name ) );
         point.value.states[ state_label->second ] = sample.value != 0.0;
      }

      void add_sample( info_family_data& data, const sample_state& sample )
      {
         point_for( data, sample, label_set() ).value.labels = sample.labels;
      }

      void add_sample( histogram_family_data& data, const sample_state& sample )
      {
         switch( sample.component ) {
            case sample_component::bucket:
               add_bucket_sample( data, sample );
               break;
            case sample_component::count:
               point_for( data, sample ).value.count = sample.value;
               break;
            case sample_component::sum:
               point_for( data, sample ).value.sum = sample.value;
               break;
            case sample_component::created:
               point_for( data, sample ).value.created = sample.value;
               break;
            default:
               throw std::runtime_error( "invalid histogram sample " + sample.name );
         }
      }

      void add_sample( gauge_histogram_family_data& data, const sample_state& sample )
      {
         switch( sample.component ) {
            case sample_component::bucket:
               add_bucket_sample( data, sample );
               break;
            case sample_component::count:
               point_for( data, sample ).value.count = sample.value;
               break;
            case sample_component::sum:
               point_for( data, sample ).value.sum = sample.value;
               break;
            default:
               throw std::runtime_error( "invalid gaugehistogram sample " + sample.name );
         }
      }

      void add_sample( summary_family_data& data, const sample_state& sample )
      {
         switch( sample.component ) {
            case sample_component::value: {
               const auto label = sample.labels.find( "quantile" );
               if( label == sample.labels.end() ) {
                  throw std::runtime_error( "summary quantile without quantile label" );
               }
               const double q = parse_label_number( label->second );
               point_for( data, sample, labels_without( sample, "quantile" ) ).value.quantiles[ q ] = sample.value;
               break;
            }
            case sample_component::count:
               point_for( data, sample ).value.count = sample.value;
               break;
            case sample_component::sum:
               point_for( data, sample ).value.sum = sample.value;
               break;
            case sample_component::created:
               point_for( data, sample ).value.created = sample.value;
               break;
            default:
               throw std::runtime_error( "invalid summary sample " + sample.name );
         }
      }
   };

   template< typename Rule >
   struct unescape
      : pegtl::nothing< Rule >
   {};

   template<>
   struct unescape< rules::normal_char >
      : pegtl::unescape_append
   {};

   template<>
   struct unescape< rules::backslash_char >
   {
      template< typename ActionInput, typename... States >
      static void apply( const ActionInput& in, std::string& out, States&&... /*unused*/ )
      {
         if( in.peek_char() == 'n' ) {
            out += '\n';
         }
         else {
            out.append( in.string_view() );
         }
      }
   };

   template< typename Rule >
   struct action
      : pegtl::nothing< Rule >
   {};

   using sample_labels = pegtl::repeat_for<
      rules::sample_labels,
      pegtl::multi_for<
         rules::sample_label,
         rules::sample_label_name,
         pegtl::unescape_for< rules::sample_label_value, unescape > > >;

   template<>
   struct action< rules::type_descriptor >
      : pegtl::multi_to<
           &builder::type_descriptor,
           rules::descriptor_metric_name,
           pegtl::cases<
              pegtl::case_< rules::counter, metric_type::counter >,
              pegtl::case_< rules::gauge, metric_type::gauge >,
              pegtl::case_< rules::histogram, metric_type::histogram >,
              pegtl::case_< rules::gaugehistogram, metric_type::gaugehistogram >,
              pegtl::case_< rules::stateset, metric_type::stateset >,
              pegtl::case_< rules::info, metric_type::info >,
              pegtl::case_< rules::summary, metric_type::summary >,
              pegtl::case_< rules::unknown, metric_type::unknown > > >
   {};

   template<>
   struct action< rules::unit_descriptor >
      : pegtl::multi_to<
           &builder::unit_descriptor,
           rules::descriptor_metric_name,
           rules::unit_string >
   {};

   template<>
   struct action< rules::help_descriptor >
      : pegtl::multi_to<
           &builder::help_descriptor,
           rules::descriptor_metric_name,
           pegtl::unescape_for< rules::help_string, unescape > >
   {};

   template<>
   struct action< rules::sample >
      : pegtl::multi_to<
           &builder::sample,
           rules::sample_metric_name,
           sample_labels,
           rules::sample_number,
           rules::sample_timestamp >
   {};

   [[nodiscard]] inline metric_set parse( const std::filesystem::path& path )
   {
      builder b;
      pegtl::file_input< pegtl::lazy::lf > in( path );
      pegtl::parse< pegtl::must< rules::exposition >, action >( in, b );
      return std::move( b ).get();
   }

}  // namespace open_metrics

#endif
