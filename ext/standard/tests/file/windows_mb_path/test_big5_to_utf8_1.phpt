--TEST--
Test mkdir/rmdir big5 to UTF-8 path 
--SKIPIF--
<?php
include dirname(__FILE__) . DIRECTORY_SEPARATOR . "util.inc";

skip_if_not_win();
if (getenv("SKIP_SLOW_TESTS")) die("skip slow test");
skip_if_no_required_exts();

?>
--FILE--
<?php
/*
#vim: set fileencoding=big5
#vim: set encoding=big5
*/

include dirname(__FILE__) . DIRECTORY_SEPARATOR . "util.inc";

$prefix = create_data("dir_big5");
$path = $prefix . DIRECTORY_SEPARATOR . "測試多字節路徑5"; // BIG5 string
$pathw = iconv('big5', 'utf-8', $path);

$subpath = $path . DIRECTORY_SEPARATOR . "測試多字節路徑4";
$subpathw = iconv('big5', 'utf-8', $subpath);

/* The mb dirname exists*/
var_dump(file_exists($pathw));

var_dump(mkdir($subpathw));
var_dump(file_exists($subpathw));

get_basename_with_cp($subpathw, 65001);

var_dump(rmdir($subpathw));
remove_data("dir_big5");

?>
===DONE===
--EXPECTF--	
bool(true)
bool(true)
bool(true)
Active code page: 65001
getting basename of %s\皜祈岫憭�摮�蝭�頝臬��5\皜祈岫憭�摮�蝭�頝臬��4
string(22) "皜祈岫憭�摮�蝭�頝臬��4"
bool(true)
string(%d) "%s\皜祈岫憭�摮�蝭�頝臬��5\皜祈岫憭�摮�蝭�頝臬��4"
Active code page: %d
bool(true)
===DONE===
