#!/usr/bin/php
<?php

define('INPUT_FILE', 'message');
define('OUTPUT_FILE', 'message_crypted');
define('KEY_FILE', 'key');
define('MODE', MCRYPT_MODE_ECB);

$key = file_get_contents(KEY_FILE);
$input = file_get_contents(INPUT_FILE);

file_put_contents(OUTPUT_FILE, mcrypt_encrypt(MCRYPT_DES, $key, $input, MODE));
