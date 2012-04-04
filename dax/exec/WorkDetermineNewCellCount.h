//=============================================================================
//
//  Copyright (c) Kitware, Inc.
//  All rights reserved.
//  See LICENSE.txt for details.
//
//  This software is distributed WITHOUT ANY WARRANTY; without even
//  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
//  PURPOSE.  See the above copyright notice for more information.
//
//  Copyright 2012 Sandia Corporation.
//  Under the terms of Contract DE-AC04-94AL85000 with Sandia Corporation,
//  the U.S. Government retains certain rights in this software.
//
//=============================================================================
#ifndef __dax_exec_WorkDetermineNewCellCount_h
#define __dax_exec_WorkDetermineNewCellCount_h

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
/// Worklet that determines how many new cells should be generated
/// from it with the same topology.
/// This worklet is based on the WorkMapCell type so you have access to
/// "CellArray" information i.e. information about what points form a cell.
/// There are different versions for different cell types, which might have
/// different constructors because they identify topology differently.

template<class CT> class WorkDetermineNewCellCount
{
public:
  typedef CT CellType;
  typedef typename CellType::TopologyType TopologyType;

private:
  CellType Cell;
  dax::exec::FieldCell<dax::Id> NewCellCount;
  dax::exec::internal::ErrorHandler ErrorHandler;
public:

  DAX_EXEC_EXPORT WorkDetermineNewCellCount(
    const TopologyType &gridStructure,
    const dax::exec::FieldCell<dax::Id> &cellCount,
    const dax::exec::internal::ErrorHandler &errorHandler)
    : Cell(gridStructure, 0),
      NewCellCount(cellCount),
      ErrorHandler(errorHandler)
    { }

  DAX_EXEC_EXPORT const CellType GetCell() const
  {
    return this->Cell;
  }

  //Set the number of cells you want this cell to generate
  DAX_EXEC_EXPORT void SetNewCellCount(dax::Id value)
  {
    dax::exec::internal::fieldAccessNormalSet(this->NewCellCount,
                                              this->GetCellIndex(),
                                              value);
  }

  template<typename T>
  DAX_EXEC_EXPORT
  const T &GetFieldValue(const dax::exec::FieldCell<T> &field) const
  {
    return dax::exec::internal::fieldAccessNormalGet(field,
                                                     this->GetCellIndex());
  }

  template<typename T>
  DAX_EXEC_EXPORT dax::Tuple<T,CellType::NUM_POINTS> GetFieldValues(
      const dax::exec::FieldPoint<T> &field) const
  {
    return dax::exec::internal::fieldAccessNormalGet<T,CellType::NUM_POINTS>(
          field, this->Cell.GetPointIndices());
  }

  DAX_EXEC_EXPORT dax::Vector3 GetFieldValue(
      const dax::exec::FieldCoordinates &, dax::Id vertexIndex) const
  {
    dax::Id pointIndex = this->GetCell().GetPointIndex(vertexIndex);
    const TopologyType &GridTopology = this->GetCell().GetGridTopology();
    return
      dax::exec::internal::fieldAccessUniformCoordinatesGet(GridTopology,
                                                            pointIndex);
  }

  DAX_EXEC_EXPORT dax::Tuple<dax::Vector3,CellType::NUM_POINTS> GetFieldValues(
    const dax::exec::FieldCoordinates &) const
  {
    const TopologyType &gridStructure = this->GetCell().GetGridTopology();
    return dax::exec::internal::fieldAccessUniformCoordinatesGet<
        TopologyType,
        dax::Vector3,
        CellType::NUM_POINTS
        > (gridStructure, this->Cell.GetPointIndices());
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

#endif //__dax_exec_WorkDetermineNewCellCount_h