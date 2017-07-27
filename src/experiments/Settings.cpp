//  Settings.cpp
//
//  Author:
//       Antonio J. Nebro <antonio@lcc.uma.es>
//
//  Copyright (c) 2011 Antonio J. Nebro, Juan J. Durillo
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU Lesser General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include <Settings.h>

/**
 * Default constructor
 */
Settings::Settings ()
{
} // Settings

/**
 * Constructor
 */
Settings::Settings(char * problemName)
{
    problemName_ = problemName;
} // Settings

/**
 * Destructor
 */
Settings::~Settings() { /* do nothing */ }

/**
 * Configure method
 *
 * To be implemented. Missing the reflection features of Java here
 */
Algorithm * Settings::configure(map<string, void *> settings)
{
    return nullptr ;
} // configure

