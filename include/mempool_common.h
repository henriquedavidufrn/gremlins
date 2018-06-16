#ifndef MEMPOOL_COMMON_H
#define MEMPOOL_COMMON_H

#include <cstdlib>          // std::malloc(), std::free()

#include "StoragePool.h"

struct Tag
{
    StoragePool * pool ;
};

void * operator new( size_t bytes )
{
    // Aloca com um espaço extra para guardar o Gerenciador de mémoria usado pelo ponteiro
    Tag * const tag = reinterpret_cast<Tag *> (std::malloc(bytes + sizeof(Tag)));
    // Gm nulo
    tag->pool = nullptr;
    // Retorna o ponteiro sem o endereço do GM
    return ( reinterpret_cast<void *> ( tag + 1U ) );
}

void * operator new( size_t bytes, StoragePool & p )
{
    // Aloca com um espaço extra para guardar o Gerenciador de mémoria usado pelo ponteiro
	Tag * const tag = reinterpret_cast<Tag *> (p.Allocate(bytes + sizeof(Tag)) );
    // Guarda o GM
	tag->pool = &p;
    // Retorna o ponteiro sem o endereço do GM
    return ( reinterpret_cast<void *> ( tag + 1U ) );
}

void operator delete( void * ptr) noexcept
{
    // Obtém o ponteiro para o começo do gerenciador de mémoria
    Tag * const tag = reinterpret_cast<Tag *>( ptr ) - 1U;
    // se tiver um gerenciador de mémoria. Libera o ponteiro dele.
    if( tag->pool != nullptr )
        tag->pool->Free( tag );
    // caso o contrário usa o free do STL
    else
        std::free( tag );
}


#endif
