#ifndef OPENMM_CMMOTIONREMOVERIMPL_H_
#define OPENMM_CMMOTIONREMOVERIMPL_H_

/* -------------------------------------------------------------------------- *
 *                                   OpenMM                                   *
 * -------------------------------------------------------------------------- *
 * This is part of the OpenMM molecular simulation toolkit originating from   *
 * Simbios, the NIH National Center for Physics-Based Simulation of           *
 * Biological Structures at Stanford, funded under the NIH Roadmap for        *
 * Medical Research, grant U54 GM072970. See https://simtk.org.               *
 *                                                                            *
 * Portions copyright (c) 2008 Stanford University and the Authors.           *
 * Authors: Peter Eastman                                                     *
 * Contributors:                                                              *
 *                                                                            *
 * Permission is hereby granted, free of charge, to any person obtaining a    *
 * copy of this software and associated documentation files (the "Software"), *
 * to deal in the Software without restriction, including without limitation  *
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,   *
 * and/or sell copies of the Software, and to permit persons to whom the      *
 * Software is furnished to do so, subject to the following conditions:       *
 *                                                                            *
 * The above copyright notice and this permission notice shall be included in *
 * all copies or substantial portions of the Software.                        *
 *                                                                            *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR *
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,   *
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL    *
 * THE AUTHORS, CONTRIBUTORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,    *
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR      *
 * OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE  *
 * USE OR OTHER DEALINGS IN THE SOFTWARE.                                     *
 * -------------------------------------------------------------------------- */

#include "ForceImpl.h"
#include "CMMotionRemover.h"
#include "Kernel.h"

namespace OpenMM {

/**
 * This is the internal implementation of CMMotionRemover.
 */

class CMMotionRemoverImpl : public ForceImpl {
public:
    CMMotionRemoverImpl(CMMotionRemover& owner);
    void initialize(OpenMMContextImpl& context);
    CMMotionRemover& getOwner() {
        return owner;
    }
    void updateContextState(OpenMMContextImpl& context);
    void calcForces(OpenMMContextImpl& context, Stream& forces) {
        // This force doesn't apply forces to atoms.
    }
    double calcEnergy(OpenMMContextImpl& context) {
        return 0.0; // This force doesn't contribute to the potential energy.
    }
    std::map<std::string, double> getDefaultParameters() {
        return std::map<std::string, double>(); // This force doesn't define any parameters.
    }
    std::vector<std::string> getKernelNames();
private:
    CMMotionRemover& owner;
    Kernel kernel;
};

} // namespace OpenMM

#endif /*OPENMM_CMMOTIONREMOVERIMPL_H_*/
