<?php

/*

  getNode(nodePath)
     getNode('/app_server/app61')

  setNode(nodePath, array)
     setNode('app_server', array('instanceName' => 'app61', ...))

  getKey(nodePath, key)
     getKey('app_server/app61', 'instanceName')

  setKey(nodePath, key, mixed)
     setKey('app_server/app61/', 'instanceName', 'app61')
 

 */

class FileDB {

  const NODEFILE = '/.node';
  const ROOTNODE = '.storage/';
  private $dir;

  public function getNode($path) {
    $content = file_get_contents(self::ROOTNODE . $path . self::NODEFILE);
    $data = array();
    $data = json_decode($content, true);
    return $data;
  }

  public function getNodeChildren($path) {
    $children = array();
    $dirs = scandir(self::ROOTNODE . $path);
    foreach($dirs as $dir) {
      if(is_dir(self::ROOTNODE . $path .'/'. $dir) && $dir != '..' && $dir != '.') { 
	array_push($children, $path .'/'. $dir);
      }
    }
    return $children;
  }

  public function getKey($path, $key) {
    $data = $this->getNode($path);
    return $data[$key];
  }

  public function createNode($path) {
    if(is_dir(self::ROOTNODE . $path)) return true;
    mkdir(self::ROOTNODE . $path, 0777, true);
    $data = array('keys' => array(), 'children' => array());
    $content = json_encode($data['keys']);
    file_put_contents(self::ROOTNODE . $path . self::NODEFILE, $content);
  }

  public function setKey($path, $key, $value) {
    $data = $this->getNode($path);
    
    $data[$key] = $value;
    $content = json_encode($data);
    
    file_put_contents(self::ROOTNODE . $path . self::NODEFILE, $content);
  }

}


?>