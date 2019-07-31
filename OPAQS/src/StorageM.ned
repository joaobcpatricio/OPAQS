//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 
//
// @author :  João Patrício (castanheirapatricio@ua.pt)
// @date   :  3-jul-2019
//

package opaqs;

simple StorageM
{
    parameters:

        // size of cache in bytes - zero means unlimited caching
        //int maximumCacheSize @unit(byte) = default(5000000byte);


        // icon used
        @display("i=block/queue");

        // statistics and signals
        @signal[cacheBytesRemoved](type=long);
        @signal[cacheBytesAdded](type=long);
        @signal[cacheBytesUpdated](type=long);
        
    

}