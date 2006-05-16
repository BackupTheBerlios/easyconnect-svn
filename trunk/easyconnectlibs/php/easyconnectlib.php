<?php
/***********************************************************************
/
/ Copyright (C) 2005, 2006 Benedikt Sauter
/
/ This program is free software; you can redistribute it and/or
/ modify it under the terms of the GNU General Public License
/ as published by the Free Software Foundation; either version 2
/ of the License, or (at your option) any later version.
/
/ This program is distributed in the hope that it will be useful,
/ but WITHOUT ANY WARRANTY; without even the implied warranty of
/ MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
/ GNU General Public License for more details.
/
/ You should have received a copy of the GNU General Public License
/ along with this program; if not, write to the Free Software
/ Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
/
***********************************************************************/

class easyConnect
{
  var $connection;
  var $debug;

  /// create a new connection
  function easyConnect($_host="localhost",$_port="9090")
  {
    $this->debug=1;
    $this->Connect($_host,$_port);
  }


  /// connect to server
  function Connect($_host,$_port)
  {
    $this->connection = @fsockopen($_host, $_port, $errno, $errstr, 1000);

    if (!$this->connection) {
      if($this->debug)
	echo "$errstr ($errno)<br />\n";
      return false;
    } else {
      return true;
    }
  }


  /// close the current connection
  function Disconnect()
  {
    if($this->connection)
    {
      fclose($this->connection); 
      return true;
    }
    else
    {
      if($this->debug)
	echo "can`t close connection because it doesn`t exists<br />\n";
      return false;
    }
  }

  function DebugLevel($level)
  {


  }

  /// discovering  server APIs

  /// returns all available functions from the sever in an array
  function ListMethods()
  {
    $result = $this->Call("ListFunctions");
    $methodlist = split('\n',$result);

    return $methodlist;
  }


  /// get some infos about a method
  function MethodHelp($method)
  {


  }


  /// send the command to the server and returns the result as an string
  function Call($command)
  {
    $result = "";
    @fwrite($this->connection, $command);
    /*
    while (!feof($this->connection)) 
    {
      $line = fgets($this->connection, 4096);
      if($line =='\n')
	break;
      $result .= $line; 
    }
    */
    $result = @fgets($this->connection,4096);
    return $result;
  }


}
?>
