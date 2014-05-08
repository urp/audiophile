# pragma once

# include <memory>
# include <string>
# include <map>
# include <typeinfo>
# include <typeindex>
# include "math.hpp"
# include "factory.hpp"

namespace audiophile
{

  namespace model
  {
    class GameObject 
    {
      public:

        GameObject( const std::string& = "untitled GameObject" );

        struct Metadata
        {
          virtual ~Metadata(){};
          std::type_info extension_type;
        };

        // TODO: virtual functiona
        template< typename ExtensionType >
        void registerMetadataType( const std::shared_ptr< Metadata >& );

        template< typename ExtensionType >
        std::shared_ptr< Metadata > getMetadata();

        virtual sphere< distance_type, 2 >      getBoundingSphere() const = 0;
        virtual box< distance_type, 2 >         getBoundingBox()    const = 0;
        //virtual convex_hull_2d< distance_type > getConvexHull()     const = 0;

      private:

        std::map< std::type_index, std::shared_ptr< Metadata > > _metadata; 
        std::string _name;

    }; // GameObject

  } // model::
} // audiophile::

// implementation //

template< typename ExtensionType >
void audiophile::model::GameObject::registerMetadataType( const std::shared_ptr< Metadata >& m )
{
  if( _metadata.find( typeid( ExtensionType ) ) == _metadata.end() )
    _metadata.insert( { typeid( ExtensionType ), m } );
  else
    if( typeid( _metadata[ typeid( ExtensionType ) ] ) != typeid( m ) )
      throw std::logic_error( "GameObject::registerMetadataType: Metadata for same key but with different type was already added." );
}

template< typename ExtensionType >
std::shared_ptr< audiophile::model::GameObject::Metadata > audiophile::model::GameObject::getMetadata()
{
  auto found = _metadata.find( typeid( ExtensionType ) );
  if( found == _metadata.end() )
    throw std::out_of_range( std::string( "audiophile::model::GameObject::getMetadata: Could not find metadata for extension type \"" ) + typeid( ExtensionType ).name() + std::string( "\"." ) );
  return found->second;
}

