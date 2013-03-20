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
#ifndef __dax_openmp_cont_internal_SetThrustForOpenMP_h
#define __dax_openmp_cont_internal_SetThrustForOpenMP_h

#include <dax/internal/Configure.h>

#ifdef DAX_ENABLE_THRUST

#if (THRUST_MAJOR_VERSION == 1 && THRUST_MINOR_VERSION >= 6)


#ifndef THRUST_DEVICE_SYSTEM
#define THRUST_DEVICE_SYSTEM THRUST_DEVICE_SYSTEM_OMP
#else // defined THRUST_DEVICE_BACKEND
#if THRUST_DEVICE_SYSTEM != THRUST_DEVICE_SYSTEM_OMP
#error Thrust device backend set incorrectly.
#endif // THRUST_DEVICE_SYSTEM != THRUST_DEVICE_SYSTEM_CUDA
#endif // defined(THRUST_DEVICE_SYSTEM)



#else //THRUST_MAJOR_VERSION == 1 && THRUST_MINOR_VERSION >= 6

#ifndef THRUST_DEVICE_BACKEND
#define THRUST_DEVICE_BACKEND THRUST_DEVICE_BACKEND_OMP
#else // defined THRUST_DEVICE_BACKEND
#if THRUST_DEVICE_BACKEND != THRUST_DEVICE_BACKEND_OMP
#error Thrust device backend set incorrectly.
#endif // THRUST_DEVICE_BACKEND != THRUST_DEVICE_BACKEND_OMP
#endif // defined THRUST_DEVICE_BACKEND


#endif //THRUST_MAJOR_VERSION == 1 && THRUST_MINOR_VERSION >= 6



#endif //DAX_ENABLE_THRUST

#endif //__dax_openmp_cont_internal_SetThrustForOpenMP_h
