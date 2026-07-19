// Copyright (c) 2026 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_SRC_EXAMPLE_OPEN_METRICS_MODEL_HPP
#define TAO_PEGTL_SRC_EXAMPLE_OPEN_METRICS_MODEL_HPP

#include <cstdint>
#include <map>
#include <optional>
#include <string>
#include <utility>
#include <variant>
#include <vector>

namespace open_metrics
{
   enum class metric_type : std::uint8_t
   {
      unknown = 0,
      gauge,
      counter,
      stateset,
      info,
      histogram,
      gaugehistogram,
      summary
   };

   using label_set = std::map< std::string, std::string >;

   struct unknown_value
   {
      static constexpr metric_type type = metric_type::unknown;

      double value = 0.0;
   };

   struct gauge_value
   {
      static constexpr metric_type type = metric_type::gauge;

      double value = 0.0;
   };

   struct counter_value
   {
      static constexpr metric_type type = metric_type::counter;

      double total = 0.0;
      std::optional< double > created;
   };

   struct histogram_value
   {
      static constexpr metric_type type = metric_type::histogram;

      std::optional< double > sum;
      std::optional< double > count;
      std::optional< double > created;
      std::map< double, double > buckets;
   };

   struct gauge_histogram_value
   {
      static constexpr metric_type type = metric_type::gaugehistogram;

      std::optional< double > sum;
      std::optional< double > count;
      std::map< double, double > buckets;
   };

   struct state_set_value
   {
      static constexpr metric_type type = metric_type::stateset;

      std::map< std::string, bool > states;
   };

   struct info_value
   {
      static constexpr metric_type type = metric_type::info;

      label_set labels;
   };

   struct summary_value
   {
      static constexpr metric_type type = metric_type::summary;

      std::optional< double > sum;
      std::optional< double > count;
      std::optional< double > created;
      std::map< double, double > quantiles;
   };

   template< typename Value >
   struct metric_point
   {
      using value_type = Value;

      metric_point() = default;

      explicit metric_point( std::optional< double > timestamp )
         : timestamp( std::move( timestamp ) )
      {}

      Value value;
      std::optional< double > timestamp;
   };

   template< typename Value >
   struct metric
   {
      using value_type = Value;
      using point_type = metric_point< Value >;

      static constexpr metric_type type = Value::type;

      metric() = default;

      explicit metric( label_set labels )
         : labels( std::move( labels ) )
      {}

      label_set labels;
      std::vector< point_type > metric_points;
   };

   template< typename Value >
   struct metric_family_data
   {
      using value_type = Value;
      using metric_type_value = metric< Value >;

      static constexpr metric_type type = Value::type;

      std::vector< metric_type_value > metrics;
   };

   using unknown_family_data = metric_family_data< unknown_value >;
   using gauge_family_data = metric_family_data< gauge_value >;
   using counter_family_data = metric_family_data< counter_value >;
   using state_set_family_data = metric_family_data< state_set_value >;
   using info_family_data = metric_family_data< info_value >;
   using histogram_family_data = metric_family_data< histogram_value >;
   using gauge_histogram_family_data = metric_family_data< gauge_histogram_value >;
   using summary_family_data = metric_family_data< summary_value >;

   using metric_family_data_value = std::variant<
      unknown_family_data,
      gauge_family_data,
      counter_family_data,
      state_set_family_data,
      info_family_data,
      histogram_family_data,
      gauge_histogram_family_data,
      summary_family_data >;

   struct metric_family
   {
      metric_family( std::string name, metric_family_data_value data )
         : name( std::move( name ) ),
           data( std::move( data ) )
      {}

      std::string name;
      std::optional< std::string > unit;
      std::optional< std::string > help;
      metric_family_data_value data;
   };

   [[nodiscard]] inline metric_type type_of( const metric_family& family )
   {
      return std::visit( []( const auto& data ) { return data.type; }, family.data );
   }

   struct metric_set
   {
      std::map< std::string, metric_family > metric_families;
   };

}  // namespace open_metrics

#endif
