/* Copyright (C) 2002 Patrick Eriksson <Patrick.Eriksson@rss.chalmers.se>
                      Stefan Buehler   <sbuehler@uni-bremen.de>
                      Claudia Emde     <claudia@sat.physik.uni-bremen.de>
                            
   This program is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by the
   Free Software Foundation; either version 2, or (at your option) any
   later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307,
   USA. */



/*****************************************************************************
 ***  File description 
 *****************************************************************************/

/*!
  \file   m_cloudbox.cc
  \author Patrick Eriksson
  \date   2002-05-08 

  \brief  Workspace functions releated to the cloud box.

  These functions are listed in the doxygen documentation as entries of the
  file auto_md.h.
*/



/*****************************************************************************
 *** External declarations
 *****************************************************************************/

#include <stdexcept>
#include "arts.h"
#include "array.h"
#include "check_input.h"
#include "matpackI.h"



/*****************************************************************************
 *** The functions (in alphabetical order)
 *****************************************************************************/

//! cloudboxSetManually
/*!
   See the the online help (arts -d FUNCTION_NAME)

   \author Patrick Eriksson
   \date   2002-05-19
*/
void cloudboxSetManually(
        // WS Output:
        Index&          cloudbox_on,
        ArrayOfIndex&   cloudbox_limits,
        // WS Input:
        const Index&    atmosphere_dim,
        const Vector&   p_grid,
        const Vector&   lat_grid,
        const Vector&   lon_grid,
        const Index&    blackbody_ground,
        // Control Parameters:
        const Numeric& p1,
        const Numeric& p2,
        const Numeric& lat1,
        const Numeric& lat2,
        const Numeric& lon1,
        const Numeric& lon2 )
{
  // Check existing WSV
  chk_if_in_range( "atmosphere_dim", atmosphere_dim, 1, 3 );
  chk_atm_grids( atmosphere_dim, p_grid, lat_grid, lon_grid );

  // Check keyword arguments
  if( p1 <= p2 )
    throw runtime_error( 
            "The pressure in *p1* must be bigger than the pressure in *p2*." );
  if( p1 <= p_grid[p_grid.nelem()-1] )
    throw runtime_error( "The pressure in *p1* must be larger than the "
                                                   "last value in *p_grid*." );
  if( p2 >= p_grid[0] )
    throw runtime_error( "The pressure in *p2* must be smaller than the "
                                                  "first value in *p_grid*." );
  if( atmosphere_dim >= 2 )
    {
      if( lat2 <= lat1 )
	throw runtime_error( 
	 "The latitude in *lat2* must be bigger than the latitude in *lat1*.");
      if( lat1 < lat_grid[1] )
	throw runtime_error( "The latitude in *lat1* must be >= than the "
                                               "second value in *lat_grid*." );
      if( lat2 > lat_grid[lat_grid.nelem()-2] )
	throw runtime_error( "The latitude in *lat2* must be <= than the "
                                         "next to last value in *lat_grid*." );
    }
  if( atmosphere_dim == 3 )
    {
      if( lon2 <= lon1 )
	throw runtime_error( 
       "The longitude in *lon2* must be bigger than the longitude in *lon1*.");
      if( lon1 < lon_grid[1] )
	throw runtime_error( "The longitude in *lon1* must be >= than the "
                                               "second value in *lon_grid*." );
      if( lon2 > lon_grid[lon_grid.nelem()-2] )
	throw runtime_error( "The longitude in *lon2* must be <= than the "
                                         "next to last value in *lon_grid*." );
    }

  // Set cloudbox_on
  cloudbox_on = 1;

  // Allocate cloudbox_limits
  cloudbox_limits.resize( atmosphere_dim*2 );

  // Pressure limits
  if( p1 > p_grid[1] )
    {
      cloudbox_limits[0] = 0;
    }
  else
    {
      for( cloudbox_limits[0]=1; p_grid[cloudbox_limits[0]+1]>p1; 
                                                     cloudbox_limits[0]++ ) {}
    }
  if( !blackbody_ground && cloudbox_limits[0]!=0 )
    {
      ostringstream os;
      os << "The lower vertical limit of the cloud box must be the lowest "
         << "pressure\nsurface when the ground is not a blackbody.";
      throw runtime_error( os.str() );
    }
  if( p2 < p_grid[p_grid.nelem()-2] )
    {
      cloudbox_limits[1] = p_grid.nelem() - 1;
    }
  else
    {
      for( cloudbox_limits[1]=p_grid.nelem()-2; 
                    p_grid[cloudbox_limits[1]-1]<p2; cloudbox_limits[1]-- ) {}
    }

  // Latitude limits
  if( atmosphere_dim >= 2 )
    {
      for( cloudbox_limits[2]=1; lat_grid[cloudbox_limits[2]+1]<lat1; 
                                                     cloudbox_limits[2]++ ) {}
      for( cloudbox_limits[2]=lat_grid.nelem()-2; 
                lat_grid[cloudbox_limits[3]-1]>lat2; cloudbox_limits[3]-- ) {}
    }

  // Longitude limits
  if( atmosphere_dim == 3 )
    {
      for( cloudbox_limits[4]=1; lon_grid[cloudbox_limits[4]+1]<lon1; 
                                                     cloudbox_limits[4]++ ) {}
      for( cloudbox_limits[5]=lon_grid.nelem()-2; 
                lon_grid[cloudbox_limits[5]-1]>lon2; cloudbox_limits[5]-- ) {}
    }
}



//! NoCloudbox
/*!
   See the the online help (arts -d FUNCTION_NAME)

   \author Patrick Eriksson
   \date   2002-05-11
*/
void NoCloudbox(
        // WS Output:
        Index&          cloudbox_on,
        ArrayOfIndex&   cloudbox_limits )
{
  cloudbox_on = 0;
  cloudbox_limits.resize(0);
}


