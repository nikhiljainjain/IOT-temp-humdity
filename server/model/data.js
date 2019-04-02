var mongoose = require('mongoose');

var vayuj = new mongoose.Schema({
    x : {type: Number},
    humdity: {type: Number, default: 30},
    y : {type: Number, default : 25}
});

module.exports = mongoose.model('vayujs',vayuj);