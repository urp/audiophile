# pragma once

# include <map>
# include <typeindex>

template< typename In, typename Out >
class factory_map
{
    typedef std::function< std::shared_ptr< Out > ( const std::shared_ptr< In >& ) > outer_function_type;

  public:

    // Register factory module for key/input type T.
    template< typename T >
    void register_module( const std::function< std::shared_ptr< Out > ( const std::shared_ptr< T >& ) >& inner_function )
    {
      auto outer_function = [inner_function]( const std::shared_ptr< In >& in ) -> std::shared_ptr< Out >
                            { 
                              auto derived_in = std::dynamic_pointer_cast< T >( in );
                              if( not derived_in ) 
                                throw std::logic_error( "factory::register_module::outer_function: Cannot cast argument of type " 
                                                      +  std::string( typeid( In ).name() )
                                                      + " to type "
                                                      +  std::string( typeid( T ).name() )
                                                      + "."
                                                      );
                              return inner_function( derived_in );
                            };

      auto ins_result = _modules.insert( { typeid( T ), outer_function } );
      if( not ins_result.second )
        throw std::logic_error( "factory::register_module: A module with key " + std::string( typeid( T ).name() ) + " is already registered." );
    }

    // Call module for derived type of *in.
    std::shared_ptr< Out > create_for( const std::shared_ptr< In >& in )
    {
      auto found = _modules.find( typeid( *in ) );
      if( found == _modules.end() )
        throw std::out_of_range( "factory::create_for: Could not find module for key \"" + std::string( typeid( *in ).name() ) + "\"." );
      return found->second( in );
    }

  private:
    std::map< std::type_index, outer_function_type > _modules;

}; // factory_map
