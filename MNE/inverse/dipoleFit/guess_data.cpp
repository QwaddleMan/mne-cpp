//=============================================================================================================
/**
* @file     guess_data.cpp
* @author   Christoph Dinh <chdinh@nmr.mgh.harvard.edu>;
*           Matti Hamalainen <msh@nmr.mgh.harvard.edu>
* @version  1.0
* @date     December, 2016
*
* @section  LICENSE
*
* Copyright (C) 2016, Christoph Dinh and Matti Hamalainen. All rights reserved.
*
* Redistribution and use in source and binary forms, with or without modification, are permitted provided that
* the following conditions are met:
*     * Redistributions of source code must retain the above copyright notice, this list of conditions and the
*       following disclaimer.
*     * Redistributions in binary form must reproduce the above copyright notice, this list of conditions and
*       the following disclaimer in the documentation and/or other materials provided with the distribution.
*     * Neither the name of MNE-CPP authors nor the names of its contributors may be used
*       to endorse or promote products derived from this software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED
* WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
* PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
* INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
* PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
* HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
* NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
* POSSIBILITY OF SUCH DAMAGE.
*
*
* @brief    Implementation of the GuessData Class.
*
*/

//*************************************************************************************************************
//=============================================================================================================
// INCLUDES
//=============================================================================================================

#include "guess_data.h"
#include "dipole_fit_data.h"






#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

#ifndef FAIL
#define FAIL -1
#endif

#ifndef OK
#define OK 0
#endif




//*************************************************************************************************************
//=============================================================================================================
// USED NAMESPACES
//=============================================================================================================

using namespace Eigen;
using namespace INVERSELIB;


//*************************************************************************************************************
//=============================================================================================================
// DEFINE MEMBER METHODS
//=============================================================================================================

GuessData::GuessData()
{

}


////*************************************************************************************************************

//GuessData::GuessData(const GuessData& p_GuessData)
//{
//}


//*************************************************************************************************************

GuessData::~GuessData()
{

}


//*************************************************************************************************************

int GuessData::compute_guess_fields(GuessData* guess,
                         DipoleFitData* f)
/*
      * Once the guess locations have been set up we can compute the fields
      */
{
    dipoleFitFuncs orig = NULL;
    int k;

    if (!guess || !f) {
        qCritical("Data missing in compute_guess_fields");
        goto bad;
    }
    if (!f->noise) {
        qCritical("Noise covariance missing in compute_guess_fields");
        goto bad;
    }
    printf("Go through all guess source locations...");
    orig = f->funcs;
    if (f->fit_mag_dipoles)
        f->funcs = f->mag_dipole_funcs;
    else
        f->funcs = f->sphere_funcs;
    for (k = 0; k < guess->nguess; k++) {
        if ((guess->guess_fwd[k] = DipoleFitData::dipole_forward_one(f,guess->rr[k],guess->guess_fwd[k])) == NULL)
            goto bad;
#ifdef DEBUG
        sing = guess->guess_fwd[k]->sing;
        printf("%f %f %f\n",sing[0],sing[1],sing[2]);
#endif
    }
    f->funcs = orig;
    printf("[done %d sources]\n",guess->nguess);
    return OK;

bad : {
        if (orig)
            f->funcs = orig;
        return FAIL;
    }
}
