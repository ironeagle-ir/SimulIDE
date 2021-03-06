/***************************************************************************
 *   Copyright (C) 2012 by santiago González                               *
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

#include "avrcomponent.h"
#include "avrprocessor.h"
#include "itemlibrary.h"
#include "mainwindow.h"


LibraryItem* AVRComponentPlugin::libraryItem()
{
    return new LibraryItem(
        tr("AVR"),
        tr("Micro"),
        "ic2.png",
        "AVR",
        AVRComponent::construct );
}

Component* AVRComponent::construct( QObject* parent, QString type, QString id )
{ 
    if( m_canCreate ) return new AVRComponent( parent, type,  id );
    
    QMessageBox* msgBox = new QMessageBox( MainWindow::self() );
    msgBox->setAttribute( Qt::WA_DeleteOnClose ); //makes sure the msgbox is deleted automatically when closed
    msgBox->setStandardButtons( QMessageBox::Ok );
    msgBox->setWindowTitle( tr("Error") );
    msgBox->setText( tr("Only 1 Mcu allowed\n to be in the Circuit.") );
    msgBox->setModal( false ); 
    msgBox->open();

    return 0l;
}

AVRComponent::AVRComponent( QObject* parent, QString type, QString id )
    : McuComponent( parent, type, id )
{
    m_pSelf = this;
    m_dataFile = "data/avrs.xml";
    m_processor = AvrProcessor::self();
    //m_processor = new AvrProcessor();
    
    /*m_t.start();
    AvrProcessor* ap = dynamic_cast<AvrProcessor*>( m_processor );
    ap->moveToThread( &m_t );*/

    if( m_id.startsWith("AVR") ) m_id.replace( "AVR", "atmega328" );

    initPackage();

    setFreq( 16 );
}
AVRComponent::~AVRComponent() { }

void AVRComponent::attachPins()
{
    AvrProcessor* ap = dynamic_cast<AvrProcessor*>( m_processor );
    avr_t* cpu = ap->getCpu();

    for( int i = 0; i < m_numpins; i++ )
    {
        AVRComponentPin* pin = dynamic_cast<AVRComponentPin*>( m_pinList[i] );
        pin->attach( cpu );
    }
    cpu->vcc  = 5000;
    cpu->avcc = 5000;
    
    // Registra IRQ para recibir petiones de voltaje de pin ( usado en ADC )
    avr_irq_t* adcIrq = avr_io_getirq( cpu, AVR_IOCTL_ADC_GETIRQ, ADC_IRQ_OUT_TRIGGER );
    avr_irq_register_notify( adcIrq, adc_hook, this );
}

void AVRComponent::addPin( QString id, QString type, QString label, int pos, int xpos, int ypos, int angle )
{
    AVRComponentPin*  newPin = new AVRComponentPin( this, id, type, label, pos, xpos, ypos, angle );
    m_pinList.append( newPin );
    
    if( type.startsWith("adc") )m_ADCpinList[type.right(1).toInt()] = newPin;
}

void AVRComponent::adcread( int channel )
{
    AVRComponentPin* pin = m_ADCpinList.value(channel);
    if( pin ) pin->adcread();
}

#include "moc_avrcomponent.cpp"
