Overview
============
QWorkBreak v1.1 is an open source minimalist Qt based work break reminder utility.

QWorkBreak shows a work break reminder notification pop-up every hour.
If user accepts the work break, he will see 5-minute long work break progress bar.
During this time user is encouraged to do some excersises, take a cup of tea/coffe or just walk around.
User can postpone a work break for 5 minutes or choose to ignore it (but the notification reappear after some random period of time).

When the user locks the workstation or there is no keyboard/mouse activity for 10 minutes, 
this will be considered as a work break. 
When the user returns to work, QWorkBreak restarts notification timer (so the next notification will appear in one hour).

Work break interval, duration and inactivity timeout are tunable.

License
============

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; version 3 of the License.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>


Credits
============
Code by Leonid Azarenkov, < leonid AT rezonics DOT com >
Timer icon by Aleksandra Wolska, http://olawolska.com/
