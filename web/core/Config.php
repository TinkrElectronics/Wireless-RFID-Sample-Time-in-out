<?php
if(count(get_included_files()) ==1) exit("Direct access not permitted.");

class Config{
    static $url = "http://192.168.1.2/wireless-rfid-sample-time-in-out/";

    static $title = "Wireless RFID Demo";

    static $database = array(
        'host' => 'localhost',
        'name' => 'demo',
        'username' => 'root',
        'password' => ''
    );
}
?>