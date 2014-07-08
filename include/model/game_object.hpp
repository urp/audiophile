# pragma once

# include <memory>
# include <string>
# include <iostream>
# include <map>
# include <typeindex>

namespace model
{
  class Game;

  class GameObject 
  {
    public:

      GameObject( const bool dynamic_flag = true, const std::string& name = "untitled GameObject" );
      // we need at least one virtual function to be polymorphic.
      virtual ~GameObject() { }

      struct Data
      {
        virtual ~Data() {}
      };

      template< typename DataType >
      void registerData( const std::shared_ptr< DataType >& );

      template< typename DataType >
      std::shared_ptr< DataType > getData();

      // dynamic flag can be used to mark object as static/dynamic in the game.
      // (this can be used as a hint to the game logic, indicating if object needs to be "advanced")
      bool is_dynamic() const { return _dynamic_flag; }

      bool is_marked_as_deleted() const { return _deleted_flag; }
      void setMarkedAsDeleted( bool del = true ) { _deleted_flag = true; }

      const std::string& name() const { return _name; }

    private:

      bool _dynamic_flag;
      bool _deleted_flag;
      std::map< std::type_index, std::shared_ptr< Data > > _data; 
      std::string _name;

  }; // GameObject

} // model::

// implementation //

template< typename DataType >
void model::GameObject::registerData( const std::shared_ptr< DataType >& d )
{
  static_assert( std::is_base_of< Data, DataType >::value, "DataType must derive from model::GameObject::Data." );

  if( _data.find( typeid( DataType ) ) == _data.end() )
    _data.insert( { typeid( DataType ), d } );
  else if( typeid( _data[ typeid( DataType ) ] ) != typeid( d ) )
    throw std::logic_error( "::model::GameObject::registerDataType: Data for same key but with different type was already added." );
  else
    std::clog << "::model::GameObject::registerDataType: " << "Warning: Data for same key and same type was already added." << std::endl;
}

template< typename DataType >
std::shared_ptr< DataType > model::GameObject::getData()
{
  auto found = _data.find( typeid( DataType ) );

  if( found == _data.end() )
  { std::clog << "::model::GameObject::getData: Could not find data of type \"" << typeid( DataType ).name() << "\"." << std::endl;
    return { nullptr };
  }

  auto data_type_ptr = std::dynamic_pointer_cast< DataType >( found->second );
  if( not data_type_ptr )
    std::clog << "::model::GameObject::getData: Could not cast to \"" << typeid( DataType ).name() << "\"." << std::endl;

  return data_type_ptr;
}

