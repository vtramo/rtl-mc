#pragma once

#include "Observable.h"

class Tile
{
public:
    Tile(Observable observable, PowersetSharedPtr patches)
        : m_observable { observable }
        , m_patches { patches }
    {
        computeClosurePatches();
        computeHashCode();
    }

    Tile(Observable observable, const Powerset& patches)
        : m_observable { observable }
        , m_patches { std::make_shared<Powerset>(patches) }
    {
        computeClosurePatches();
        computeHashCode();
    }

    Tile() = default;

    const Observable& observable() const { return m_observable; }
    PowersetConstSharedPtr patches() const { return m_patches; }
    PowersetConstSharedPtr closure() const { return m_closure; }
    std::size_t hashcode() const { return m_hashcode; }

    friend std::ostream& operator<< (std::ostream& os, const Tile& tile)
    {
        os << tile.observable() << '\n';
        os << "Tile: " << PPLUtils::toString(*tile.patches()) << "\n\n";
        return os;
    }
private:
    Observable m_observable {};
    PowersetConstSharedPtr m_patches {};
    PowersetConstSharedPtr m_closure {};
    std::size_t m_hashcode {};

    void computeClosurePatches()
    {
        if (m_patches->is_topologically_closed())
        {
            m_closure = m_patches;
        }
        else
        {
            PowersetSharedPtr closure { std::make_shared<Powerset>(*m_patches) };
            closure->topological_closure_assign();
            m_closure = closure;
        }
    }

    void computeHashCode()
    {
        m_hashcode = m_patches->hash_code();
    }
};

inline bool operator== (const Tile& tile1, const Tile& tile2)
{
    return tile1.observable() == tile2.observable() &&
           *tile1.patches() == *tile2.patches();
}

inline bool operator!= (const Tile& tile1, const Tile& tile2)
{
    return !(tile1 == tile2);
}

template<> struct std::hash<Tile>
{
    std::size_t operator() (const Tile& tile) const noexcept
    {
        return tile.hashcode();
    }
};