<?php

$dir = "/bin/";
$files = scandir($dir);
$results = array();

echo "Processing $dir ";

foreach ($files as $file) {
  if (is_file($dir . $file) && is_readable($dir . $file)) {
    echo ".";
    $fp = $dir . $file;
    $random_key = substr(sha1(rand()), 0, 6);

    $results[$fp] = array();
    $results[$fp]['key'] = $random_key;

    /* Encryption test */
    file_put_contents('/tmp/moulinette_php', mcrypt_encrypt(MCRYPT_DES, $random_key, file_get_contents($fp), MCRYPT_MODE_ECB));
    $hash_php_des = sha1(file_get_contents('/tmp/moulinette_php'));
    exec("./des -e -i $fp -o /tmp/moulinette -k $random_key");
    $hash_our_des = sha1(file_get_contents("/tmp/moulinette"));
    $results[$fp]['php_encrypted_hash'] = $hash_php_des;
    $results[$fp]['our_encrypted_hash'] = $hash_our_des;
    $results[$fp]['success'] = $hash_our_des === $hash_php_des;

    /* Decryption test */
    $hash_php_des = sha1(mcrypt_decrypt(MCRYPT_DES, $random_key, file_get_contents('/tmp/moulinette_php'), MCRYPT_MODE_ECB));
    exec("./des  -d -i /tmp/moulinette -o /tmp/moulinette_decrypt -k $random_key");
    $hash_our_des = sha1(file_get_contents("/tmp/moulinette_decrypt"));
    $results[$fp]['php_decrypted_hash'] = $hash_php_des;
    $results[$fp]['our_decrypted_hash'] = $hash_our_des;

    $results[$fp]['success'] = $results[$fp]['success'] && $hash_our_des === $hash_php_des;
  }
}

echo " done!\n";

$good = 0;
$bad = 0;

foreach ($results as $key => $arr) {
  if ($arr['success'] !== true) {
    echo $key . " failed\n";
    $bad++;
  } else {
    $good++;
  }
}

$total = $good + $bad;
echo "$good / $total pass\n";
