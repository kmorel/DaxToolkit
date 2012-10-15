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
#ifndef __dax_exec_CellWedge_h
#define __dax_exec_CellWedge_h

#include <dax/Types.h>
#include <dax/exec/internal/TopologyUnstructured.h>

namespace dax { namespace exec {

class CellWedge
{
public:

  /// static variable that holds the number of points per cell
  const static dax::Id NUM_POINTS = 6;
  typedef dax::Tuple<dax::Id,NUM_POINTS> PointConnectionsType;
  const static dax::Id TOPOLOGICAL_DIMENSIONS = 3;

private:
  PointConnectionsType Connections;

public:
  /// Create a cell for the given work.
  template<class ExecutionAdapter>
  DAX_EXEC_EXPORT explicit CellWedge(
    const dax::exec::internal::TopologyUnstructured<
      CellWedge,ExecutionAdapter> &)
    :Connections()
    { }

  /// Get the number of points in the cell.
  DAX_EXEC_EXPORT dax::Id GetNumberOfPoints() const
  {
    return NUM_POINTS;
  }

  /// Given a vertex index for a point (0 to GetNumberOfPoints() - 1), returns
  /// the index for the point in point space.
  DAX_EXEC_EXPORT dax::Id GetPointIndex(const dax::Id vertexIndex) const
  {
    return this->GetPointIndices()[vertexIndex];
  }

  /// returns the indices for all the points in the cell.
  DAX_EXEC_EXPORT
  const PointConnectionsType &GetPointIndices() const
  {
    return this->Connections;
  }

  //  method to set this cell from a portal
  template<class ConnectionsPortalT>
  DAX_EXEC_EXPORT void SetPointIndices(
    const dax::exec::internal::TopologyUnstructured<
        CellWedge,ConnectionsPortalT> &topology,
      dax::Id cellIndex)
  {
    const dax::Id offset = cellIndex*NUM_POINTS;
    this->Connections[0] = topology.CellConnections.Get(offset + 0);
    this->Connections[1] = topology.CellConnections.Get(offset + 1);
    this->Connections[2] = topology.CellConnections.Get(offset + 2);
    this->Connections[3] = topology.CellConnections.Get(offset + 3);
    this->Connections[4] = topology.CellConnections.Get(offset + 4);
    this->Connections[5] = topology.CellConnections.Get(offset + 5);
  }

  //  method to set this cell from a different tuple
  DAX_EXEC_EXPORT void SetPointIndices(
      const PointConnectionsType & cellConnections)
  {
    this->Connections = cellConnections;
  }

  // A COPY CONSTRUCTOR IS NEEDED TO OVERCOME THE SLOWDOWN DUE TO NVCC'S DEFAULT
  // COPY CONSTRUCTOR.
  DAX_EXEC_EXPORT CellWedge(const CellWedge& wge)
  :Connections(wge.Connections)
  {}

private:
  // MAKING SURE THAT THERE ARE NO MORE ASSIGNMENTS HAPPENING THAT WILL
  // POTENTIALLY BRING ABOUT A PERFOMANCE HIT
  CellWedge & operator = (CellWedge other);

};

}}
#endif
