#pragma once

#include "ObservablePatchSequence.h"

/*!
 *  \brief A read-only view of a slice of an `ObservablePatchSequence`.
 *
 * This class provides a lightweight, read-only view of a subsequence of an `ObservablePatchSequence`.
 * It contains a reference to the original sequence and defines a range within it using `startIndex` and `endIndex`.
 * The class is designed to avoid unnecessary copies and improve efficiency, particularly when used with
 * operators like \ref traverseZero and \ref traversePlus.
 *
 * The slice represents a contiguous subsequence of the original sequence, starting at `startIndex` (inclusive) and
 * ending at `endIndex` (inclusive). It provides methods to query properties of the slice, such as its
 * size, space dimension, and the first observable patch or interpretation in the slice.
 *
 * \warning This class is only a view and does not own the underlying data. Its lifetime must be less than or equal to
 * the lifetime of the referenced \ref ObservablePatchSequence. Accessing the view after the original sequence
 * has been destroyed results in undefined behavior.
 *
 * \note This class is immutable and does not modify the underlying \ref ObservablePatchSequence.
 */
class ObservablePatchSequenceSlice
{
public:
    /*!
     * \brief Constructs a slice of an \ref ObservablePatchSequence.
     * \param observablePatchSequence The original sequence to create a slice from.
     * \param startIndex The starting index of the slice (inclusive).
     * \param endIndex The ending index of the slice (inclusive).
     */
    ObservablePatchSequenceSlice(const ObservablePatchSequence& observablePatchSequence, unsigned startIndex, unsigned endIndex);

    /*!
     * \brief Checks if the slice is empty.
     * \return `true` if the slice is empty, `false` otherwise.
     */
    [[nodiscard]] bool isEmpty() const;

    /*!
     * \brief Returns the total number of patches in the slice.
     * \return The number of patches in the slice.
     */
    [[nodiscard]] int totalPatches() const;

    /*!
     * \brief Returns the space dimension of the \ref ObservablePatchSequence.
     * \return The space dimension of the patches, as defined by the underlying \ref ObservablePatchSequence.
     */
    [[nodiscard]] PPL::dimension_type spaceDimension() const;

    /*!
     *  \brief Returns the first \ref ObservablePatch in the slice, if it exists.
     *  \return An `std::optional` containing a pointer to the first \ref ObservablePatch in the slice,
     *          or `std::nullopt` if the slice is empty.
     */
    [[nodiscard]] std::optional<const ObservablePatch* const> firstObservablePatch() const;

    /*!
     *  \brief Returns the interpretation of the observable of the first \ref ObservablePatch in the slice, if it exists.
     *  \return An `std::optional` containing a shared pointer to the first observable interpretation in the slice,
     *          or `std::nullopt` if the slice is empty or the first patch has no interpretation.
     */
    [[nodiscard]] std::optional<PowersetConstSharedPtr> firstObservableInterpretation() const;

    /*!
     * \brief Advances the start index of the slice by one, creating a new slice.
     * \return A new `ObservablePatchSequenceSlice` with the advanced start index.
     */
    [[nodiscard]] ObservablePatchSequenceSlice advanceStartIndexByOne() const;

    /*!
     * \brief Returns the starting index of the slice.
     * \return The starting index of the slice (inclusive).
     */
    [[nodiscard]] unsigned startIndex() const;

    /*!
     * \brief Returns the ending index of the slice.
     * \return The ending index of the slice (inclusive).
     */
    [[nodiscard]] unsigned endIndex() const;

private:
    const ObservablePatchSequence& m_observablePatchSequence; //!< Reference to the original sequence.
    const unsigned m_startIndex {}; //!< Starting index of the slice (inclusive).
    const unsigned m_endIndex {}; //!< Ending index of the slice (inclusive).
};

/*!
 * \brief Computes the set of points from which there exists a left-closed admissibile trajectory that traverse the sequence \ref ObservablePatchSequence
 * \param sequence The sequence of patches to traverse.
 * \param preFlow A convex polyhedron representing a pre-flow.
 * \return A powerset representing the set of points from which there exists a left-closed admissible trajectory that traverse the sequence.
 */
PowersetSharedPtr traverseZero(const ObservablePatchSequence& sequence, const Poly& preFlow);

/*!
 * \overload
 * \see traverseZero(const ObservablePatchSequence& sequence, const Poly& preFlow)
 */
PowersetSharedPtr traverseZero(ObservablePatchSequenceSlice slice, const Poly& preFlow);

/*!
 * \brief Computes the set of points from which there exists a left-open admissible trajectory that traverse the sequence \ref ObservablePatchSequence
 * \param sequence The sequence of patches to traverse.
 * \param preFlow A convex polyhedron representing a pre-flow.
 * \return A powerset representing the set of points from which there exists a left-open admissible trajectory that traverse the sequence.
 */
PowersetSharedPtr traversePlus(const ObservablePatchSequence& sequence, const Poly& preFlow);

/*!
 * \overload
 * \see traversePlus(const ObservablePatchSequence& sequence, const Poly& preFlow)
 */
PowersetSharedPtr traversePlus(ObservablePatchSequenceSlice slice, const Poly& preFlow);