# pragma once

# include <map>
# include <typeindex>

namespace audiophile
{
  template< typename In, typename Out >
  class factory_map
  {
      typedef std::function< std::shared_ptr< Out > ( const std::shared_ptr< In >& ) > module_function_type;

    public:

      // Register factory module for key/input type T.
      template< typename T >
      void register_module( const module_function_type& f )
      {
        auto ins_result = _modules.insert( { typeid( T ), f } );
        if( not ins_result.second )
          throw std::logic_error( "factory::register_module: A module with key " + std::string( typeid( T ).name() ) + " is already registered." );
      }

      // Find module for derived type of *in.
      std::shared_ptr< Out > create_for( const std::shared_ptr< In >& in )
      {
        auto found = _modules.find( typeid( *in ) );
        if( found == _modules.end() )
          throw std::out_of_range( "factory::create_for: Could not find module for key \"" + std::string( typeid( *in ).name() ) + "\"." );
        return found->second( in );
      }

    private:
      std::map< std::type_index, module_function_type > _modules;

  }; // factory_map

} // audiophile::
