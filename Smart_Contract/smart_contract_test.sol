// This Smart Contract can store a data a data array of 8*32 bytes
// this array can contain a data hash (256 bits) for example 
pragma solidity ^0.4.18;


contract Sample {
    uint8[32] data;
    bytes8[32] dataBytes;

    function my_set(uint8[32] d) public{
      for (uint i = 0; i < 32; i++) {
          data[i] = d[i];
      }
    }

    function my_get() public constant returns (uint) {
        
	    return 0;
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
