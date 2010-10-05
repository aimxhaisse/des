#!/usr/bin/php
<?php

define('CRYPTED_FILE', 'message_crypted');
define('KEY_FILE', 'key');
define('MODE', MCRYPT_MODE_ECB);

$key = file_get_contents(KEY_FILE);
$input = file_get_contents(CRYPTED_FILE);

echo mcrypt_decrypt(MCRYPT_DES, $key, $input, MODE);
