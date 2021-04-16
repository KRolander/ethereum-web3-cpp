var Tx = require('ethereumjs-tx')
const Web3 = require('web3')
const Web3Utils = require('web3-utils');
var Eth = require('web3-eth');


const web3 = new Web3(new Web3.providers.HttpProvider('http://127.0.0.1:7545'))

const pricateKeyStr = '4bf790d8eb389b31dbe9fe8d357792e61577ed34810b5481c802219afe814e0a'
const privateKey = Buffer.from(pricateKeyStr, 'hex')

const from = '0xc9E6181F370fFA6c44707C60Dd5a1B317A6DDBF2'

web3.eth.defaultAccount =  '0xc9E6181F370fFA6c44707C60Dd5a1B317A6DDBF2'

const contractAddress = '0x0154B359b49fE8790A5397A88213d4C99B9aA15C' //'0x44b72e86bc2dA10BaaDFA6571c3B044dABeb0769' // 



 


const contractABI = [
	{
		"constant": false,
		"inputs": [
			{
				"name": "_num",
				"type": "uint8"
			}
		],
		"name": "toByte",
		"outputs": [
			{
				"name": "_ret",
				"type": "bytes1"
			}
		],
		"payable": false,
		"stateMutability": "nonpayable",
		"type": "function"
	},
	{
		"constant": true,
		"inputs": [],
		"name": "my_get_bytes",
		"outputs": [
			{
				"name": "",
				"type": "bytes8[32]"
			}
		],
		"payable": false,
		"stateMutability": "view",
		"type": "function"
	},
	{
		"constant": false,
		"inputs": [
			{
				"name": "d",
				"type": "uint8[32]"
			}
		],
		"name": "my_set",
		"outputs": [],
		"payable": false,
		"stateMutability": "nonpayable",
		"type": "function"
	},
	{
		"constant": true,
		"inputs": [],
		"name": "my_get",
		"outputs": [
			{
				"name": "",
				"type": "uint256"
			}
		],
		"payable": false,
		"stateMutability": "view",
		"type": "function"
	}
]
// const tab = [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31] 
const tab  = [0xdb, 0x8a, 0x04, 0x22, 0x24, 0xc4, 0x4b, 0x05, 0xa9, 0x7e, 0x5f, 0x2a, 0x41, 0x0e, 0xa6, 0x04, 0xd8, 0x18, 0xbb, 0xe9, 0xe6, 0xa5, 0xd2, 0xbe, 0xed, 0x57, 0x78, 0xe7, 0x9e, 0xfd, 0x4a, 0xcf]

const encodedFonction = web3.sha3('my_set(uint8[32])').substring(0, 10)
console.log('encode function name', encodedFonction)


// //////////////////////////////////////////////////////////////////////////////////////////
const ethereumV = new Eth()
// const encodedParam = ethereumV.abi.encodeParameter('uint', 1)
// encodedParam = ethereumV.abi.encodeFunctionSignature('get()')
// console.log('Encoded Parameters ', encodedParam)

encodedParam2 = ethereumV.abi.encodeParameter('uint8[32]',tab)
console.log('Encoded Parameters2 ', encodedParam2)

const SmartContractPayload = encodedFonction + encodedParam2.substring(2, encodedParam2.lenght)
console.log('Smart Contract Payload', SmartContractPayload)

const txObject = {
    nonce: '0x6',
    from: from,
    gasPrice: '0x4A817C800', //'0x4A817C801',
    gas: '0x6691B7', 
    to: contractAddress,
    // value: '0xde0b6b3a7640000',
           
    data: SmartContractPayload
}
 
const tx = new Tx(txObject)
tx.sign(privateKey)


const serializedTx = tx.serialize()
const raw = '0x' + serializedTx.toString('hex')
console.log('raw : ',raw)



web3.eth.sendRawTransaction(raw, (err, txHash) => {
    console.log('txHash:', txHash)
})

// //////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Call if the tab was filled 

// var dappTestContract = web3.eth.contract(contractABI, contractAddress)
var dappTestContract = web3.eth.contract(contractABI).at(contractAddress)

var contract =  new web3.eth.contract(contractABI,contractAddress);

// Use THIS : 
// const abiEncoded = dappTestContract.my_get(function(error, result) {console.log('result ' + result, 'error ' + error)})
// const abiEncoded = dappTestContract.my_set(2) //.call(function(error, result) {console.log('result ' + result, 'error ' + error)})
// const abiEncoded = dappTestContract.my_set(tab) //.call(function(error, result) {console.log('result ' + result, 'error ' + error)})
 
const abiEncoded = dappTestContract.my_get_bytes(function(error, result) {console.log('result ' + result, 'error ' + error)})

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


// const txFromHash = web3.eth.getTransaction('0x0dde72faaac2494ef3361aa30fed6493cdcc186ff061db28538bde514eb92017')
// console.log('Transaction from hash ', txFromHash)

// const tx_data = '0xe6523c0616ce4c0e401f0078b554017c6863587602630e1b130a376ef15c98d0' //txFromHash.inputs;
// console.log('tx_data ', tx_data)



// const input_data = '0x' + tx_data.substring(0, 10)

// const tx_params = ethereumV.abi.decodeParameter('bool', input_data)
// console.log(' Results ', tx_params)





// const txObject = {
//     nonce: '0x0',
//     gasPrice: '0x4A817C800',
//     gasLimit: '0x6691B7',
//     // to: '0xC78E793b56b7ab3CC7B4ec4dB3471343585a06Ad',
//     // value: '0xde0b6b3a7640000',
//     data: '0x6060604052341561000f57600080fd5b61010d8061001e6000396000f3006060604052600436106049576000357c0100000000000000000000000000000000000000000000000000000000900463ffffffff16806360fe47b114604e5780636d4ce63c14606e575b600080fd5b3415605857600080fd5b606c60048080359060200190919050506098565b005b3415607857600080fd5b607e60a2565b604051808215151515815260200191505060405180910390f35b8060008190555050565b600060405180807f4f4b000000000000000000000000000000000000000000000000000000000000815250600201905060405180910390a060019050905600a165627a7a723058209362bee176273fa9aa78e2593f1e8d3de4f5cd1409567f43c67b662a1a3039130029'
// }

// web3.eth.getBalance('0x21133d23E04e58E846c2fA45fD5F2b75Cf91B204', (err, wei) => { balance = web3.fromWei(wei, 'ether'); console.log(balance.toString())})
// const jsw = '0xf1018504a817c800836691b794c78e793b56b7ab3cc7b4ec4db3471343585a06ad880de0b6b3a7640000846d4ce63c1c8080'            
                
// const cpp = '0xf1018504a817c800836691b794c78e793b56b7ab3cc7b4ec4db3471343585a06ad88de0b6b3a76400000846d4ce63c1c8080'



//  ################# Sign and Send Transaction #####################
// const tx = new Tx(txObject)
// tx.sign(privateKey)

// const serializedTx = tx.serialize()
// const raw = '0x' + serializedTx.toString('hex')
// console.log('raw : ',raw)

// web3.eth.sendRawTransaction(raw, (err, txHash) => {
//     console.log('txHash:', txHash)
// })

// var Tx = require('ethereumjs-tx');
// var privateKey = new Buffer('e331b6d69882b4cb4ea581d88e0b604039a3de5967688d3dcffdd2270c0fd109', 'hex')

// var rawTx = {
//   nonce: '0x00',
//   gasPrice: '0x09184e72a000',
//   gasLimit: '0x2710',
//   to: '0x0000000000000000000000000000000000000000',
//   value: '0x00',
//   data: '0x7f7465737432000000000000000000000000000000000000000000000000000000600057'
// }
// 09 18 4e 72 a0 00


// var tx = new Tx(rawTx);
// tx.sign(privateKey);

// var serializedTx = tx.serialize();

// // console.log(serializedTx.toString('hex'));
// // 0xf889808609184e72a00082271094000000000000000000000000000000000000000080a47f74657374320000000000000000000000000000000000000000000000000000006000571ca08a8bbf888cfa37bbf0bb965423625641fc956967b81d12e23709cead01446075a01ce999b56a8a88504be365442ea61239198e23d1fce7d00fcfc5cd3b44b7215f

// web3.eth.sendSignedTransaction('0x' + serializedTx.toString('hex'))
// .on('receipt', console.log);

// > // see eth.getTransactionReceipt() for details
