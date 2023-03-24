<?php

echo "_SERVER VARIABLES:\n";
foreach ($_SERVER as $key => $value) {
	echo "_SERVER[$key] = $value\n";
}

echo "\n_GET VARIABLES:\n";
foreach ($_GET as $key => $value) {
	echo "_GET[$key] = $value\n";
}

echo "\n_POST VARIABLES:\n";
foreach ($_POST as $key => $value) {
	echo "_POST[$key] = $value\n";
}

echo "\n_REQUEST VARIABLES:\n";
foreach ($_REQUEST as $key => $value) {
	echo "_REQUEST[$key] = $value\n";
}

// echo "\nHTTP_RAW_POST_DATA VARIABLES= $HTTP_RAW_POST_DATA\n";

// echo "\nHTTP_RAW_GET_DATA VARIABLES= $HTTP_RAW_GET_DATA\n";

// $envvv = getenv();
// foreach ($envvv as $key => $value) {
// 	echo "envvv[$key] = $value\n";
// }


?>