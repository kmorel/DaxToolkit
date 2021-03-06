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

// This test makes sure that the algorithms specified in
// DeviceAdapterAlgorithmGeneral.h are working correctly. It does this by
// creating a test device adapter that uses the serial device adapter for the
// base schedule/scan/sort algorithms and using the general algorithms for
// everything else. Because this test is based of the serial device adapter,
// make sure that UnitTestDeviceAdapterSerial is working before trying to debug
// this one.

#define DAX_DEVICE_ADAPTER DAX_DEVICE_ADAPTER_ERROR

#include <dax/cont/DeviceAdapterSerial.h>
#include <dax/cont/internal/DeviceAdapterAlgorithmGeneral.h>

#include <dax/cont/internal/testing/TestingDeviceAdapter.h>

namespace dax {
namespace cont {
namespace internal {

struct DeviceAdapterTagTestAlgorithmGeneral { };

template <typename T, class ArrayContainerControlTag>
class ArrayManagerExecution
    <T,
    ArrayContainerControlTag,
    dax::cont::internal::DeviceAdapterTagTestAlgorithmGeneral>
    : public dax::cont::internal::ArrayManagerExecution
          <T, ArrayContainerControlTag, dax::cont::DeviceAdapterTagSerial>
{
public:
  typedef dax::cont::internal::ArrayManagerExecution
      <T, ArrayContainerControlTag, dax::cont::DeviceAdapterTagSerial>
      Superclass;
  typedef typename Superclass::ValueType ValueType;
  typedef typename Superclass::PortalType PortalType;
  typedef typename Superclass::PortalConstType PortalConstType;
};

template<>
struct DeviceAdapterAlgorithm<
           dax::cont::internal::DeviceAdapterTagTestAlgorithmGeneral> :
    dax::cont::internal::DeviceAdapterAlgorithmGeneral<
        DeviceAdapterAlgorithm<
                   dax::cont::internal::DeviceAdapterTagTestAlgorithmGeneral>,
        dax::cont::internal::DeviceAdapterTagTestAlgorithmGeneral>
{
private:
  typedef dax::cont::internal::DeviceAdapterAlgorithm<
      dax::cont::DeviceAdapterTagSerial> Algorithm;

public:

  template<class Functor>
  DAX_CONT_EXPORT static void Schedule(Functor functor,
                                       dax::Id numInstances)
  {
    Algorithm::Schedule(functor, numInstances);
  }

  template<class Functor>
  DAX_CONT_EXPORT static void Schedule(Functor functor,
                                       dax::Id3 rangeMax)
  {
    Algorithm::Schedule(functor, rangeMax);
  }

  template<typename T, class CIn, class COut>
  DAX_CONT_EXPORT static T ScanInclusive(
      const dax::cont::ArrayHandle<T,CIn,DeviceAdapterTagTestAlgorithmGeneral> &input,
      dax::cont::ArrayHandle<T,COut,DeviceAdapterTagTestAlgorithmGeneral>& output)
  {
    // Need to use array handles compatible with serial adapter.
    dax::cont::ArrayHandle<T,CIn,DeviceAdapterTagSerial>
        inputCopy(input.GetPortalConstControl());
    dax::cont::ArrayHandle<T,COut,DeviceAdapterTagSerial> originalOutput;

    T result = Algorithm::ScanInclusive(inputCopy, originalOutput);

    // Copy data back into original
    originalOutput.CopyInto(output.GetPortalControl().GetIteratorBegin());

    return result;
  }

  template<typename T, class CIn, class COut>
  DAX_CONT_EXPORT static T ScanExclusive(
      const dax::cont::ArrayHandle<T,CIn,DeviceAdapterTagTestAlgorithmGeneral> &input,
      dax::cont::ArrayHandle<T,COut,DeviceAdapterTagTestAlgorithmGeneral>& output)
  {
    // Need to use array handles compatible with serial adapter.
    dax::cont::ArrayHandle<T,CIn,DeviceAdapterTagSerial>
        inputCopy(input.GetPortalConstControl());
    dax::cont::ArrayHandle<T,COut,DeviceAdapterTagSerial> originalOutput;

    T result = Algorithm::ScanExclusive(inputCopy, originalOutput);

    // Copy data back into original
    originalOutput.CopyInto(output.GetPortalControl().GetIteratorBegin());

    return result;
  }

  template<typename T, class Container>
  DAX_CONT_EXPORT static void Sort(
      dax::cont::ArrayHandle<T,Container,DeviceAdapterTagTestAlgorithmGeneral>& values)
  {
    // Need to use an array handle compatible with the serial adapter.
    dax::cont::ArrayHandle<T,Container,DeviceAdapterTagSerial> valuesCopy;
    // Allocate memory in valuesCopy (inefficiently).
    valuesCopy.PrepareForOutput(values.GetNumberOfValues());
    values.CopyInto(valuesCopy.GetPortalControl().GetIteratorBegin());

    Algorithm::Sort(valuesCopy);

    // Copy data back into original
    valuesCopy.CopyInto(values.GetPortalControl().GetIteratorBegin());
  }

  template<typename T, class Container, class Compare>
  DAX_CONT_EXPORT static void Sort(
      dax::cont::ArrayHandle<T,Container,DeviceAdapterTagTestAlgorithmGeneral>& values,
      Compare comp)

  {
    // Need to use an array handle compatible with the serial adapter.
    dax::cont::ArrayHandle<T,Container,DeviceAdapterTagSerial> valuesCopy;
    // Allocate memory in valuesCopy (inefficiently).
    valuesCopy.PrepareForOutput(values.GetNumberOfValues());
    values.CopyInto(valuesCopy.GetPortalControl().GetIteratorBegin());

    Algorithm::Sort(valuesCopy,comp);

    // Copy data back into original
    valuesCopy.CopyInto(values.GetPortalControl().GetIteratorBegin());
  }

  DAX_CONT_EXPORT static void Synchronize()
  {
    Algorithm::Synchronize();
  }
};

}
}
} // namespace dax::cont::internal

int UnitTestDeviceAdapterAlgorithmGeneral(int, char *[])
{
  return dax::cont::internal::TestingDeviceAdapter
      <dax::cont::internal::DeviceAdapterTagTestAlgorithmGeneral>::Run();
}
