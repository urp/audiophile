# pragma once

# include <map>
# include <typeindex>

template< typename InBase, typename Out >
class factory_map
{
    typedef std::function< std::shared_ptr< Out > ( const std::shared_ptr< InBase >& ) > outer_function_type;

  public:

    // Register factory module for key/input type In.
    template< typename In >
    void register_module( const std::function< std::shared_ptr< Out > ( const std::shared_ptr< In >& ) >& inner_function )
    {
      auto outer_function = [inner_function]( const std::shared_ptr< InBase >& in ) -> std::shared_ptr< Out >
                            {
                              static_assert( std::is_base_of< InBase, In >::value, "Key type \"In\" must derive from factory_maps template parameter \"InBase\"." );
                              auto derived_in = std::static_pointer_cast< In >( in );
                              return inner_function( derived_in );
                            };
      auto ins_result = _modules.insert( { typeid( In ), outer_function } );
      if( not ins_result.second )
        throw std::logic_error( "factory_map::register_module: A module with key \"" + std::string( typeid( In ).name() ) + "\" is already registered." );
    }

    // Call module for derived type of *in.
    std::shared_ptr< Out > create_for( const std::shared_ptr< InBase >& in )
    {
      auto found = _modules.find( typeid( *in ) );
      if( found == _modules.end() )
        throw std::out_of_range( "factory_map::create_for: Could not find module for key \"" + std::string( typeid( *in ).name() ) + "\"." );
      return found->second( in );
    }

  private:
    std::map< std::type_index, outer_function_type > _modules;

}; // factory_map
