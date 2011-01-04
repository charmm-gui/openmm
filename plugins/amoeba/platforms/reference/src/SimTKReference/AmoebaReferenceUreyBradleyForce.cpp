
/* Portions copyright (c) 2006 Stanford University and Simbios.
 * Contributors: Pande Group
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject
 * to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS, CONTRIBUTORS OR COPYRIGHT HOLDERS BE
 * LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 * OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include "AmoebaReferenceUreyBradleyForce.h"
#include "AmoebaReferenceForce.h"

using std::vector;
using OpenMM::RealVec;

/**---------------------------------------------------------------------------------------

   Calculate Amoeba UB ixn (force and energy)

   @param positionAtomA           Cartesian coordinates of atom A
   @param positionAtomB           Cartesian coordinates of atom B
   @param idealLength             ideal length
   @param kForceConstant          force constant
   @param cubicK                  cubic force parameter
   @param quarticK                quartic force parameter
   @param forces                  force vector

   @return energy

   --------------------------------------------------------------------------------------- */

RealOpenMM AmoebaReferenceUreyBradleyForce::calculateUreyBradleyIxn( const RealVec& positionAtomA, const RealVec& positionAtomB,
                                                                     RealOpenMM idealLength, RealOpenMM kForceConstant,
                                                                     RealOpenMM cubicK, RealOpenMM quarticK,
                                                                     RealVec* forces ) const {

   // ---------------------------------------------------------------------------------------

   //static const std::string methodName = "AmoebaReferenceUreyBradleyForce::calculateHarmonicForce";

   static const RealOpenMM zero          = 0.0;
   static const RealOpenMM one           = 1.0;
   static const RealOpenMM onePt5        = 1.5;
   static const RealOpenMM two           = 2.0;

   // ---------------------------------------------------------------------------------------

   // get deltaR, R2, and R between 2 atoms

   std::vector<RealOpenMM> deltaR;
   AmoebaReferenceForce::loadDeltaR( positionAtomA, positionAtomB, deltaR );
   RealOpenMM r               = AmoebaReferenceForce::getNorm3( deltaR );

   // deltaIdeal = r - r_0

   RealOpenMM deltaIdeal      = r - idealLength;
   RealOpenMM deltaIdeal2     = deltaIdeal*deltaIdeal;

   RealOpenMM dEdR            = (one + onePt5*cubicK*deltaIdeal + two*quarticK*deltaIdeal2);
   dEdR                      *= two*kForceConstant*deltaIdeal;
   dEdR                       = r > zero ? (dEdR/r) : zero;

   forces[0][0]              += dEdR*deltaR[0];
   forces[0][1]              += dEdR*deltaR[1];
   forces[0][2]              += dEdR*deltaR[2];

   forces[1][0]              -= dEdR*deltaR[0];
   forces[1][1]              -= dEdR*deltaR[1];
   forces[1][2]              -= dEdR*deltaR[2];

   RealOpenMM energy          = kForceConstant*deltaIdeal2*( one + cubicK*deltaIdeal + quarticK*deltaIdeal2 );
   return energy;
}

RealOpenMM AmoebaReferenceUreyBradleyForce::calculateForceAndEnergy( int numIxns,
                                                                      vector<RealVec>& particlePositions,
                                                                      const std::vector<int>&   particle1,
                                                                      const std::vector<int>&   particle2,
                                                                      const std::vector<RealOpenMM>& length,
                                                                      const std::vector<RealOpenMM>& kQuadratic,
                                                                      RealOpenMM globalUreyBradleyCubic,
                                                                      RealOpenMM globalUreyBradleyQuartic,
                                                                      vector<RealVec>& forceData ) const {
    RealOpenMM energy      = 0.0; 
    for( int ii = 0; ii < numIxns; ii++ ){
        int particle1Index            = particle1[ii];
        int particle2Index            = particle2[ii];
        RealOpenMM idealLength        = length[ii];
        RealOpenMM kForceConstant     = kQuadratic[ii];
        RealVec forces[2];
        forces[0]                     = forceData[particle1Index];
        forces[1]                     = forceData[particle2Index];
        energy                       += calculateUreyBradleyIxn( particlePositions[particle1Index], particlePositions[particle2Index],
                                                                 idealLength, kForceConstant, globalUreyBradleyCubic, globalUreyBradleyQuartic,
                                                                 forces );
    }   
    return energy;
}
