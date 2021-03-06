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
#ifndef __dax_cont_arg_FieldArrayHandleCounting_h
#define __dax_cont_arg_FieldArrayHandleCounting_h

#include <dax/Types.h>
#include <dax/cont/arg/ConceptMap.h>
#include <dax/cont/arg/Field.h>
#include <dax/cont/ArrayHandle.h>
#include <dax/cont/sig/Tag.h>
#include <dax/exec/arg/FieldPortal.h>
#include <dax/internal/Tags.h>

#include <boost/mpl/if.hpp>

namespace dax { namespace cont { namespace arg {

/// \headerfile FieldArrayHandle.h dax/cont/arg/FieldArrayHandle.h
/// \brief Map array of a single constant value to \c Field worklet parameters.
template <typename Tags, typename Device>
class ConceptMap< Field(Tags), dax::cont::ArrayHandleCounting<Device> > :
  public ConceptMap< Field(Tags), dax::cont::ArrayHandle < dax::Id,
                                  dax::cont::ArrayContainerControlTagCounting,
                                  Device > >
{
  typedef ConceptMap< Field(Tags), dax::cont::ArrayHandle < dax::Id,
                                  dax::cont::ArrayContainerControlTagCounting,
                                  Device > > superclass;
  typedef dax::cont::ArrayHandleCounting<Device> HandleType;
public:
  ConceptMap(HandleType handle):
    superclass(handle)
    {}
};

/// \headerfile FieldArrayHandle.h dax/cont/arg/FieldArrayHandle.h
/// \brief Map array of a single constant value to \c Field worklet parameters.
template <typename Tags, typename Device>
class ConceptMap< Field(Tags), const dax::cont::ArrayHandleCounting<Device> > :
  public ConceptMap< Field(Tags), const dax::cont::ArrayHandle < dax::Id,
                                  dax::cont::ArrayContainerControlTagCounting,
                                  Device > >
{
  typedef ConceptMap< Field(Tags), const dax::cont::ArrayHandle < dax::Id,
                                  dax::cont::ArrayContainerControlTagCounting,
                                  Device > > superclass;
  typedef dax::cont::ArrayHandleCounting<Device> HandleType;
public:
  ConceptMap(HandleType handle):
    superclass(handle)
    {}
};


} } } //namespace dax::cont::arg

#endif //__dax_cont_arg_FieldArrayHandleCounting_h
