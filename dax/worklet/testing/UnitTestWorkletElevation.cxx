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

#define DAX_ARRAY_CONTAINER_CONTROL DAX_ARRAY_CONTAINER_CONTROL_BASIC
#define DAX_DEVICE_ADAPTER DAX_DEVICE_ADAPTER_SERIAL

#include <dax/cont/internal/testing/TestingGridGenerator.h>
#include <dax/cont/internal/testing/Testing.h>

#include <dax/worklet/Elevation.h>

#include <dax/cont/ArrayHandle.h>
#include <dax/cont/DeviceAdapter.h>
#include <dax/cont/Scheduler.h>
#include <dax/cont/UniformGrid.h>

#include <dax/math/VectorAnalysis.h>

#include <vector>

namespace {

const dax::Id DIM = 64;

//-----------------------------------------------------------------------------
struct TestElevationWorklet
{
  //----------------------------------------------------------------------------
  template<typename GridType>
  void operator()(const GridType&) const
  {
  dax::cont::internal::TestGrid<GridType> grid(DIM);
  dax::Id numPoints = grid->GetNumberOfPoints();

  dax::cont::ArrayHandle<dax::Scalar,
                         dax::cont::ArrayContainerControlTagBasic,
                         dax::cont::DeviceAdapterTagSerial> elevationHandle;

  dax::Vector3 maxCoordinate = grid.GetPointCoordinates(numPoints-1);
  dax::Scalar scale = 0.5/dax::math::MagnitudeSquared(maxCoordinate);

  std::cout << "Running Elevation worklet" << std::endl;
  dax::worklet::Elevation elev(-1.0*maxCoordinate, maxCoordinate);
  dax::cont::Scheduler<> scheduler;
  scheduler.Invoke(elev,
                   grid->GetPointCoordinates(),
                   elevationHandle);

  std::cout << "Checking result" << std::endl;
  std::vector<dax::Scalar> elevation(grid->GetNumberOfPoints());
  elevationHandle.CopyInto(elevation.begin());
  for (dax::Id pointIndex = 0; pointIndex < numPoints; pointIndex++)
    {
    dax::Scalar elevationValue = elevation[pointIndex];
    dax::Vector3 pointCoordinates = grid.GetPointCoordinates(pointIndex);
    dax::Scalar elevationExpected =
        scale * dax::dot(pointCoordinates, maxCoordinate) + 0.5;
    DAX_TEST_ASSERT(test_equal(elevationValue, elevationExpected),
                    "Got bad elevation.");
    }
  }
};

//-----------------------------------------------------------------------------
void TestElevation()
  {
  dax::cont::internal::GridTesting::TryAllGridTypes(TestElevationWorklet());
  }



} // Anonymous namespace

//-----------------------------------------------------------------------------
int UnitTestWorkletElevation(int, char *[])
{
  return dax::cont::internal::Testing::Run(TestElevation);
}
