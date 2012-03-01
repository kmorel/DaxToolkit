/*=========================================================================

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.  See the above copyright notice for more information.

===========================================================================*/
#ifndef __dax_exec_WorkRemoveCell_h
#define __dax_exec_WorkRemoveCell_h

#include <dax/Types.h>
#include <dax/exec/Cell.h>
#include <dax/exec/Field.h>
#include <dax/exec/WorkMapCell.h>

#include <dax/internal/GridTopologys.h>
#include <dax/exec/internal/ErrorHandler.h>
#include <dax/exec/internal/FieldAccess.h>

namespace dax {
namespace exec {

///----------------------------------------------------------------------------
// Work for worklets that determine if a cell should be removed. This
// worklet is based on the WorkMapCell type so you have access to
// "CellArray" information i.e. information about what points form a cell.
// There are different versions for different cell types, which might have
// different constructors because they identify topology differently.

template<class CT> class WorkRemoveCell
{
public:
  typedef CT CellType;
  typedef typename CellType::TopologyType TopologyType;
  typedef char MaskType;

private:
  CellType Cell;
  dax::exec::FieldCell<MaskType> DeadCells;
  dax::exec::FieldPoint<MaskType> DeadPoints;
  dax::exec::internal::ErrorHandler ErrorHandler;
public:

  DAX_EXEC_EXPORT WorkRemoveCell(
    const TopologyType &gridStructure,
    const dax::exec::FieldCell<MaskType> &deadCells,
    const dax::exec::FieldPoint<MaskType> &deadPoints,
    const dax::exec::internal::ErrorHandler &errorHandler)
    : Cell(gridStructure, 0),
      DeadCells(deadCells),
      DeadPoints(deadPoints),
      ErrorHandler(errorHandler)
    { }

  DAX_EXEC_EXPORT const CellType GetCell() const
  {
    return this->Cell;
  }

  //set this to true if you want to remove this cell
  //Any cell with the value of zero is removed.
  DAX_EXEC_EXPORT void SetRemoveCell(char value)
  {
    dax::exec::internal::fieldAccessNormalSet(this->DeadCells,
                                              this->GetCellIndex(),
                                              value);
    if(value)
      {
      //TODO: Make this a separate function
      //This is currently okay to be in an if statement
      //since Threshold very rarely branches inside a wavelet, but
      //that won't always be the case
      dax::Id pointIndices[CellType::NUM_POINTS];
      this->GetCell().GetPointIndices(pointIndices);

      //also set each point of that cell to be removed.
      for(dax::Id i=0;i<CellType::NUM_POINTS;++i)
        {
        dax::exec::internal::fieldAccessNormalSet(this->DeadPoints,
                                                  pointIndices[i],
                                                  value);
        }
      }
  }

  //set this to true if you want to remove this cell
  DAX_EXEC_EXPORT dax::Id IsCellRemoved()
  {
    return dax::exec::internal::fieldAccessNormalGet(this->DeadCells,
                                                     this->GetCellIndex());
  }

  template<typename T>
  DAX_EXEC_EXPORT
  const T &GetFieldValue(const dax::exec::FieldCell<T> &field) const
  {
    return dax::exec::internal::fieldAccessNormalGet(field,
                                                     this->GetCellIndex());
  }

  template<typename T>
  DAX_EXEC_EXPORT const T &GetFieldValue(const dax::exec::FieldPoint<T> &field,
                                         dax::Id vertexIndex) const
  {
    dax::Id pointIndex = this->GetCell().GetPointIndex(vertexIndex);
    return dax::exec::internal::fieldAccessNormalGet(field, pointIndex);
  }

  template<typename T>
  DAX_EXEC_EXPORT void GetFieldValues(const dax::exec::FieldPoint<T> &field,
                                          T values[CellType::NUM_POINTS]) const
  {
    dax::Id pointIndices[CellType::NUM_POINTS];
    this->GetCell().GetPointIndices(pointIndices);
    dax::exec::internal::fieldAccessNormalGet<CellType::NUM_POINTS>(field, pointIndices, values);
  }

  DAX_EXEC_EXPORT dax::Vector3 GetFieldValue(
    const dax::exec::FieldCoordinates &, dax::Id vertexIndex) const
  {
    dax::Id pointIndex = this->GetCell().GetPointIndex(vertexIndex);
    const TopologyType &gridStructure = this->GetCell().GetGridTopology();
    return
        dax::exec::internal::fieldAccessUniformCoordinatesGet(gridStructure,
                                                              pointIndex);
  }

  DAX_EXEC_EXPORT void GetFieldValues(
    const dax::exec::FieldCoordinates &,
      dax::Vector3 coords[CellType::NUM_POINTS]) const
  {
    dax::Id indices[CellType::NUM_POINTS];
    this->GetCell().GetPointIndices(indices);

    const TopologyType &gridStructure = this->GetCell().GetGridTopology();
    dax::exec::internal::fieldAccessUniformCoordinatesGet<CellType::NUM_POINTS>(gridStructure, indices, coords);
  }

  DAX_EXEC_EXPORT dax::Id GetCellIndex() const { return this->Cell.GetIndex(); }

  DAX_EXEC_EXPORT void SetCellIndex(dax::Id cellIndex)
  {
    this->Cell.SetIndex(cellIndex);
  }

  DAX_EXEC_EXPORT void RaiseError(const char* message)
  {
    this->ErrorHandler.RaiseError(message);
  }  
};


}
}

#endif //__dax_exec_WorkRemoveCell_h
