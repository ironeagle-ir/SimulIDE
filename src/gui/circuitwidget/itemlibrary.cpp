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

#include "itemlibrary.h"
#include "itemlibiface.h"

//BEGIN Item includes
#include "buffer.h"
#include "capacitor.h"
#include "diode.h"
//#include "FlipFlopJK.h"
//#include "FlipFlopD.h"
#include "gate_and.h"
#include "gate_or.h"
#include "gate_xor.h"
#include "ground.h"
#include "hd44780.h"
#include "inductor.h"
#include "led.h"
#include "logicinput.h"
#include "op_amp.h"
//#include "socket.h"
#include "pcd8544.h"
#include "probe.h"
#include "potentiometer.h"
#include "push.h"
//#include "rampa.h"
#include "resistor.h"
#include "sevensegment.h"
#include "steeper.h"
#include "switch.h"
#include "voltsource.h"
//#include "avrcomponent.h"
#ifndef NO_PIC
//#include "piccomponent.h"
#endif
//#include "avrboard.h"
#include "subcircuit.h"
//END Item includes


ItemLibrary::ItemLibrary()
{
    loadItems();
    loadPlugins();
}
ItemLibrary::~ItemLibrary(){}

void ItemLibrary::loadItems()
{
    m_items.clear();
        // Sources
    m_items.append( LogicInput::libraryItem() );
    m_items.append( VoltSource::libraryItem() );
    m_items.append( Ground::libraryItem() );
    //switches
    m_items.append( Switch::libraryItem() );
    m_items.append( Push::libraryItem() );
    // Passive
    m_items.append( Potentiometer::libraryItem() );
    m_items.append( Resistor::libraryItem() );
    m_items.append( Capacitor::libraryItem() );
    m_items.append( Inductor::libraryItem() );
    m_items.append( Diode::libraryItem() );
        // Active
    m_items.append( OpAmp::libraryItem() );
        // Outputs
    m_items.append( Probe::libraryItem() );
    //m_items.append( Socket::libraryItem() );
    m_items.append( Led::libraryItem() );
    m_items.append( SevenSegment::libraryItem() );
    m_items.append( Hd44780::libraryItem() );
    m_items.append( Pcd8544::libraryItem() );
    m_items.append( Steeper::libraryItem() );
        // Gates
    m_items.append( Buffer::libraryItem() );
    m_items.append( AndGate::libraryItem() );
    m_items.append( OrGate::libraryItem() );
    m_items.append( XorGate::libraryItem() );
        //Subcircuits
    //m_items.append( SubCircuit::libraryItem() );
        // Micro
    //m_items.append( AVRComponent::libraryItem() );
    #ifndef NO_PIC
    //m_items.append( PICComponent::libraryItem() );
    #endif
        //Boards
    //m_items.append( AVRBoard::libraryItem() );
}

void ItemLibrary::loadPlugins()
{
    m_plugins.clear();
    QDir pluginsDir( qApp->applicationDirPath() );

    pluginsDir.cd( "plugins" );

    qDebug() << "\n    Loading Component plugins at:  "<<pluginsDir.absolutePath()<<"\n";

    QString pluginName = "*plugin.*";
    pluginsDir.setNameFilters(QStringList(pluginName));

    foreach( pluginName, pluginsDir.entryList( QDir::Files ) )
    {
        QPluginLoader pluginLoader( pluginsDir.absoluteFilePath( pluginName ) );
        QObject *plugin = pluginLoader.instance();
        
        pluginName = pluginName.split(".").first().remove("lib").remove("plugin").toUpper();

        if( plugin )
        {
            ItemLibIface *item = qobject_cast<ItemLibIface *>( plugin );
            
            LibraryItem* libItem = item->libraryItem();
            if( item && !(m_plugins.contains(pluginName)) ) 
            {
                m_items.append( libItem );
                m_plugins.append(pluginName);
                qDebug()<< "        Loaded plugin\t" << pluginName;
            }
        }
        else
            qDebug()<< "        " << pluginName << "\tplugin FAILED: " << pluginLoader.errorString();
    }
}

LibraryItem * ItemLibrary::libraryItem( QString type ) const
{
    foreach ( LibraryItem* item, m_items )
    {
        if ( item->type() == type )
            return item;
    }
    return 0l;
}


// CLASS LIBRAYITEM *********************************************************

LibraryItem::LibraryItem( const QString &name, const QString &category,
                          const QString &iconName, const QString type, createItemPtr _createItem )
{
    m_name      = name;
    m_category  = category;
    m_iconfile  = iconName;
    m_type      = type;
    createItem  = _createItem;
}
LibraryItem::~LibraryItem() { }

