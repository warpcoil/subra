//This is executed first

//When you describe a struct in JavaScript, we need to know the type

var uint8_t = 1;
var uint16_t = 2;
var uint32_t = 4;
var int8_t = -1;
var int16_t = -2;
var int32_t = -4;

/* For example

var myStruct = [
            { type: int8_t, amount: 1, name: "myIndex" },
            { type: int8_t, amount: 127, name: "myString" }
        ];

This tells us, we would like a structure with a small index value, and a fixed length string (not pointer) of 127 chars
*/

//We have described the data, but where to we put it?
//We could allocate and serialise, or place at a specific linear physical address
//Why might we want a physical address?  Hardware tables

/*For Example

var myData = {
    value: [
            { myIndex : 1, myString : "Hello" },
            { myIndex : 2, myString : "World" }
        ],
    where: null (allocated assumed)
    elements: 1 assumed
};

var ptr = serialise(myStruct, myData);

This will allocate enough space for all of the data, packed described by myStruct
In this case 0x01Hello[fill to 127 with 0]0x02World[fill to 127 with 0]

We could easily emplace the data at a specific address (no allocation takes place)
myData.where = 0xdeadbeef;
var ptr = serialise(myStruct, myData);

*/

//Getting data in is one thing, but out?

/*
myData.where = 0xdeadbeef;
deserialise(myStruct, myData);
*/
//Do something with array of data

function sizeof(obj) {

    if (typeof (obj) === "number") {
        if (!isNaN(obj)) {
            switch (obj) {
                case 1: //[u]int8_t
                case -1:
                    return 1;
                case 2: //[u]int16_t
                case -2:
                    return 2;
                case 4: //[u]int32_t
                case -4:
                    return 4;
            }
        }
    }

    if (typeof (obj) === "string") {
        return obj.length + 1; //allow null terminator
    }

    //Basic types accounted for, but what about a descriptor
    if (!Array.isArray(obj)) {
        //Expected descriptor! which is an array
        message("%s", "sizeof::Object is not an array of type descriptor");
        return 0;
    }

    var szDescriptor = 0;

    for (var i = 0; i < obj.length; i++) {

        if (!obj[i].hasOwnProperty("type")) {
            obj[i].type = -1;
        }

        if (!obj[i].hasOwnProperty("amount")) {
            obj[i].amount = 1;
        }

        switch (obj[i].type) {
            case 1:
            case -1:
                szDescriptor += (obj[i].amount);
                break;
            case 2:
            case -2:
                szDescriptor += (2 * obj[i].amount);
                break;
            case 4:
            case -4:
                szDescriptor += (4 * obj[i].amount);
                break;
            default:
                throw "sizeof:: Unsupported Data Type";
        }

    }

    return szDescriptor;

}

//Serialise
function serialise(descriptor, data) {
    //serialise data
    var alloc = false;

    if (data.where === undefined)
        alloc = true;

    if (data.where === null)
        alloc = true;

    if (alloc) {
        //Get the amount of space needed
        var size = sizeof(descriptor) * data.data.length;
        data.where = malloc(size);
    }

    var where = data.where;

    for (var i = 0; i < data.data.length; i++) {

        for (var j = 0; j < descriptor.length; j++) {

            //Check key exists in js object
            if (!data.data[i].hasOwnProperty(descriptor[j].name))
                throw "serialise:: " + descriptor[j].name + " not found";

            var func;

            switch (descriptor[j].type) {
                case -1: //int8_t or char or if amount > 1, string
                    func = writeInt8_t;
                    break;
                case -2: //int16_t or short or if amount > 1, ucs/utf string
                    func = writeInt16_t;
                    break;
                case -4: //int32_t or long / int
                    func = writeInt32_t;
                    break;
                case 1: //uint8_t or unsigned char or if amount > 1, string [unsigned extended]
                    func = writeUint8_t;
                    break;
                case 2: //uint16_t or unsigned short
                    func = writeUint16_t;
                    break;
                case 4: //uint32_t or unsigned long / int
                    func = writeUint32_t;
                    break;
                default:
                    throw "serialise:: Unknown Descriptor Type: " + descriptor[j].type;
            }

            if (typeof data.data[i][descriptor[j].name] === "string") {

                for (var k = 0; k < descriptor[j].amount; k++) {
                    if (k < data.data[i][descriptor[j].name].length)
                        func(where + k, data.data[i][descriptor[j].name].charCodeAt(k));
                    else func(where + k, 0);
                }

                where += (descriptor[j].amount * sizeof(descriptor[j].type));

            } else if (Array.isArray(data.data[i][descriptor[j].name])) {

                for (var k = 0; k < descriptor[j].amount; k++) {
                    if (k < data.data[i][descriptor[j].name].length)
                        func(where + k, data.data[i][descriptor[j].name][k]);
                    else func(where + k, 0);
                }

                where += (descriptor[j].amount * sizeof(descriptor[j].type));

            } else if (descriptor[j].amount === 1) {

                func(where, data.data[i][descriptor[j].name]);
                where += sizeof(descriptor[j].type);

            } else
                throw "serialise:: Malformed data, cannot write: " + descriptor[j].name;

        }

    }
    
    return data.where;
}

function deserialise(descriptor, where, amount) {

    if (amount === undefined)
        amount = 1;

    if (amount === null)
        amount = 1;

    var data = {};
    data.data = []; //Always accepts and returns an array
    data.where = where;
    data.amount = amount;

    for (var i = 0; i < amount; i++) { //equivalent to data.data.length

        data.data[i] = {};

        for (var j = 0; j < descriptor.length; j++) {

            var func;

            switch (descriptor[j].type) {
                case -1: //int8_t or char or if amount > 1, string
                    func = readInt8_t;
                    break;
                case -2: //int16_t or short or if amount > 1, ucs/utf string
                    func = readInt16_t;
                    break;
                case -4: //int32_t or long / int
                    func = readInt32_t;
                    break;
                case 1: //uint8_t or unsigned char or if amount > 1, string [unsigned extended]
                    func = readUint8_t;
                    break;
                case 2: //uint16_t or unsigned short
                    func = readUint16_t;
                    break;
                case 4: //uint32_t or unsigned long / int
                    func = readUint32_t;
                    break;
                default:
                    throw "serialise:: Unknown Descriptor Type: " + descriptor[j].type;
            }

            if (descriptor[j].amount === 1)
            {
                data.data[i][descriptor[j].name] = func(where);
                where += sizeof(descriptor[j].type);
            } else {

                var x = [];
                for (var k = 0; k < descriptor[j].amount; k++) {

                    x[k] = func(where);
                    where += sizeof(descriptor[j].type);
                    
                }
                data.data[i][descriptor[j].name] = x;

            }

        }

    }

    return data;
}

function arrayToString(array) {
    return String.fromCharCode.apply(null, array)
}

