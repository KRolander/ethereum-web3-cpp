// this array can contain a data hash (256 bits) for example 
pragma solidity ^0.4.18;


contract Sample {
    uint8[32] data;
    bytes8[32] dataBytes;
    
    uint256 dataIoT;
    uint8 ID;
    address public pubKeyIoT = 0xFA3cA730E9d357cc7B6D8d82E8eDF92C7c062Aec;
    
        
    bytes32 tmpHash;  
    
    bool tmp = false;

    
   function toBytes(uint256 x) public returns (bytes memory b) {
        b = new bytes(32);
        assembly { mstore(add(b, 32), x) }
    }
  
  

  
    // function setMyData(uint256 _dataToStore, uint8 _IDofIoT, uint256 _r, uint256 _s) public returns (bool) {
        
    //     bytes memory fullValueToHash = new bytes (32 + 1);
    //     bytes memory valueToHash_part1 = toBytes(_dataToStore);
        
    //     bytes32  r = bytes32(_r);
    //     bytes32  s = bytes32(_s);
        
        
    //     uint i=0;
    //     for(i=0; i<32; i++)
    //     {
    //         fullValueToHash[i] = valueToHash_part1[i];
    //     }
    //     fullValueToHash[32] = byte(_IDofIoT); // valueToHash_part2
        
        
    //     bytes32 hash = keccak256(fullValueToHash);
    //     tmpHash = hash;
        
    //     uint8 v = 27;
        
    //     if ((ecrecover(hash, v, r, s) == pubKeyIoT) || (ecrecover(hash, v+1, r, s) == pubKeyIoT)) 
    //         	return true;
    //     else
    //         	return false;
        
        
    // }
    
    
    function setMyData(uint256 _dataToStore, uint8 _IDofIoT, uint256 _r, uint256 _s) public returns (bool) {
        
        bytes memory fullValueToHash = new bytes (32 + 1);
        bytes memory valueToHash_part1 = toBytes(_dataToStore);
        
        bytes32  r = bytes32(_r);
        bytes32  s = bytes32(_s);
        
        
        uint i=0;
        for(i=0; i<32; i++)
        {
            fullValueToHash[i] = valueToHash_part1[i];
        }
        fullValueToHash[32] = byte(_IDofIoT); // valueToHash_part2
        
        
        bytes32 hash = keccak256(fullValueToHash);
        tmpHash = hash;
        
        uint8 v = 27;
        
        if ((ecrecover(hash, v, r, s) == pubKeyIoT) || (ecrecover(hash, v+1, r, s) == pubKeyIoT)) 
        {
                tmp = true;
            	return true;
        }
        else{
                tmp = false;
            	return false;
        }
        
    }
    

    function my_set(uint8[32] d) public{
      for (uint i = 0; i < 32; i++) {
          data[i] = d[i];
      }
    }

    function my_get() public constant returns (bool) {
        
	    return tmp;
    }

   

    function my_get_bytes()  public constant returns(bytes8[32]){
         for (uint i = 0; i < 32; i++) {
            dataBytes[i] = toByte(data[i]);
         }
        return dataBytes;
    }
    
    function toByte(uint8 _num) returns (byte _ret) {
        return byte(_num);
    }
     
  
}
