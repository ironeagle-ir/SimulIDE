/***************************************************************************
 *   Copyright (C) 2016 by santiago González                               *
 *   santigoro@gmail.com                                                   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, see <http://www.gnu.org/licenses/>.  *
 *                                                                         *
 ***************************************************************************/

#ifndef EMOSFET_H
#define EMOSFET_H

#include "e-resistor.h"
#include "e-source.h"

class eMosfet : public eResistor
{
    public:

        eMosfet( std::string id );
        ~eMosfet();

        void initialize();
        virtual void setVChanged();
        
        virtual bool pChannel()               { return m_Pchannel; }
        virtual void setPchannel( bool pc )   { m_Pchannel = pc; }
        
        virtual double RDSon()                { return m_RDSon; }
        virtual void  setRDSon( double rdson ){ m_RDSon = rdson; }
        
        virtual double threshold()            { return m_threshold; }
        virtual void  setThreshold( double th )
        { 
            m_threshold = th; 
            m_kRDSon = m_RDSon*(10-m_threshold);
            m_Gth = m_threshold-m_threshold/4;
        }
        
    protected:
        double m_DScurrent;
        double m_threshold;
        double m_lastGateV;
        double m_lastAdmit;
        double m_dAdmit;
        double m_cAdmit;
        double m_kRDSon;
        double m_RDSon;
        double m_Gth;
        double m_Vs;
        double m_convTh;
        
        int m_convCount;
        
        bool m_connected;
        bool m_converged;
        
        bool m_Pchannel;
        
        eSource* m_gate;
        
        
        bool m_haveMin;
        bool m_haveMax;
        double m_minA;
        double m_maxA;
        
        
        
        double m_lastDScurrent;
        double m_dewltaCurr;
        double m_totalInc;
};

#endif
