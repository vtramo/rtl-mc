/**
 * \file Tile.h
 * \brief A maximal connected set of patches of the same \c Observable
 */

#pragma once

#include "Observable.h"
#include "utils.h"

/**
 * \class Tile
 * \brief A \c Tile is a maximal connected set of patches of the same \c Observable
 *
 * Despite the fact that tiles are not necessarily convex, every point \f$x\f$ of a \c Tile
 * is reachable from any other point \f$y\f$ in same \c Tile by means of an admissible
 * trajectory that never exits the tile.
 *
 * Formally, for any two points \f$x\f$ and \f$y\f$ in a \c Tile \f$P\f$, there exists an admissible trajectory \f$f\f$
 * and a time delay \f$\delta \geq 0 \f$ such that:
 * \f[
 * f(0) = x,\, f(\delta) = y,\, \text{ and }\, f(t) \in P\, \text{for all}\, t \in (0, \delta)
 * \f]
 *
 * \note An \c Observable can have more than one \c Tile.
 *
 * \invariant All \ref Tile::patches() belong to the interpretation of the \ref Observable (\ref Observable::interpretation()).
 * \invariant The \ref Tile::patches() represent a maximal connected set of patches of the \c Observable (\ref Observable::interpretation()).
 *
 * \see TileExtractor
 * \see Observable
 */
class Tile
{
public:

    /**
     * \brief Constructs a \c Tile from an \c Observable and a maximal connected set of patches of the \c Observable
     * \param observable The \c Observable that contains the \c Tile
     * \param patches A maximal connected set of patches of the \c Observable (as a \ref PowersetConstSharedPtr)
     * \throws Abort if \p patches are not contained in the \ref Observable::interpretation() (enabled only in DEBUG mode)
     */
    Tile(Observable observable, PowersetConstSharedPtr patches)
        : m_observable { observable }
        , m_patches { patches }
    {
        assert(m_observable.interpretation()->contains(*m_patches));
        computeClosurePatches();
        computeHashCode();
    }

    /**
     * \brief Constructs a \c Tile from an \c Observable and a maximal connected set of patches of the \c Observable
     * \param observable The \c Observable that contains the \c Tile
     * \param patches A maximal connected set of patches of the \c Observable
     * \throws Abort if \p patches are not contained in the \ref Observable::interpretation() (enabled only in DEBUG mode)
     */
    Tile(Observable observable, const Powerset& patches)
        : m_observable { observable }
        , m_patches { std::make_shared<Powerset>(patches) }
    {
        assert(m_observable.interpretation()->contains(*m_patches));
        computeClosurePatches();
        computeHashCode();
    }

    Tile() = default;

    /**
     * \brief Retrieves a constant reference to the \c Observable that contains this \c Tile
     * \return A constant reference to the \c Observable
     */
    [[nodiscard]] const Observable& observable() const { return m_observable; }

    /**
     * \brief Retrieves the maximal connected set of patches of the \c Observable
     * \return A shared pointer to the maximal connected set of patches of the \c Observable
     */
    [[nodiscard]] PowersetConstSharedPtr patches() const { return m_patches; }

    /**
     * \brief Retrieves the topologically closed maximal connected set of patches of the \c Observable
     * \return A shared pointer to the topologically closed maximal connected set of patches of the \c Observable
     */
    [[nodiscard]] PowersetConstSharedPtr closure() const { return m_closure; }

    /**
     * \brief Gets the precomputed hash code
     * \return The hash value for this \c Tile
     */
    [[nodiscard]] std::size_t hashcode() const { return m_hashcode; }

    /**
     * \brief Gets the space dimension of the \c Tile
     * \return The space dimension of the \c Tile
     */
    [[nodiscard]] PPL::dimension_type spaceDimension() const { return m_observable.spaceDimension(); }

    /**
     * \brief Output stream operator for a \c Tile
     * \param os Output stream
     * \param tile \c Tile to output
     * \return Reference to the output stream
     */
    friend std::ostream& operator<< (std::ostream& os, const Tile& tile)
    {
        os << tile.observable() << '\n';
        os << "Tile: " << PPLUtils::toString(*tile.patches()) << "\n\n";
        return os;
    }
private:
    Observable m_observable {}; ///< The \c Observable that contains this \c Tile
    PowersetConstSharedPtr m_patches {}; ///< A maximal connected set of patches of the \ref m_observable
    PowersetConstSharedPtr m_closure {}; ///< A topologically closed version of \ref m_patches
    std::size_t m_hashcode {}; ///< Precomputed hash code

    /**
     * \brief Computes the topological closure of the \ref m_patches
     *
     * Stores result in \ref m_closure. If patches are already closed, just copies the pointer.
     */
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

    /**
     * /brief Computes and stores the hash code for this \c Tile
     *
     * Stores the result in \ref m_hashcode
     */
    void computeHashCode()
    {
        hashCombine(m_hashcode, m_observable, *m_patches);
    }
};

/**
 * \brief Equality comparison for \c Tile
 * \param tile1 First \c Tile to compare
 * \param tile2 Second \c Tile to compare
 * \return true if both \c Tile s have the same \ref Tile::hashcode()
 */
inline bool operator== (const Tile& tile1, const Tile& tile2)
{
    return tile1.hashcode() == tile2.hashcode();
}

/**
 * \brief Inequality comparison for \c Tile
 * \param tile1 First \c Tile to compare
 * \param tile2 Second \c Tile to compare
 * \return true if the \c Tile s are different
 */
inline bool operator!= (const Tile& tile1, const Tile& tile2)
{
    return !(tile1 == tile2);
}

/**
 * \brief Hash specialization for \c Tile
 */
template<> struct std::hash<Tile>
{
    /**
     * \brief Get the hash of a \c Tile
     * \param tile The \c Tile to take the hash from
     * \return Precomputed hash code of the \c Tile
     */
    std::size_t operator() (const Tile& tile) const noexcept
    {
        return tile.hashcode();
    }
};