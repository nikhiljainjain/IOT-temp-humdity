var mongoose = require('mongoose');
let new_date = new Date();
//let date_save = new Date(Date.UTC(x.getFullYear(),x.getMonth(),x.getDay(),x.getHours()+5,x.getMinutes()+30));
let data = new_date.getHours() + (new_date.getMinutes()/60)+(5.36);
console.log(data);

var vayuj = new mongoose.Schema({
    //time
    x : {type: Number},
    humdity: {type: Number, default: 30},
    y : {type: Number, default : 25}, //temperature
    s_date : {type: Number, default : 11},
    weather : {type: String, default: "Sunny"}
});

module.exports = mongoose.model('vayujs',vayuj);