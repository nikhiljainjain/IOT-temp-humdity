var express = require('express');
var router = express.Router();
var Data = require("../model/data.js");
const request = require('request');
const config = require("../config.js");
const options = {
	    method: 'GET',
	    url: `https://api.aerisapi.com/observations/chennai,india?&format=json&filter=allstations&limit=1&client_id=${config.access_id}&client_secret=${config.secret_key}`
    };
var led = 0;

function dataCollect(callback){
    request(options, function (error, response, body) {
    	const json = JSON.parse(body);
    		
    	if (!json.success) {
    		console.error('Oh no!');
    		callback('Oh no!');
    	} else {
    	    console.log("Calling to API");
    		//console.log(json);
    		callback(json);
    	}
    });
};


router.get('/', function(req, res, next) {
    const new_date = new Date();
    const dateX = Date.now;//new_date.getDate();
    //console.log(dateX);
    Data.find({}/*{s_date : dateX}*/, "x y humdity",(err,read)=>{
        if (err) throw err;
        const something = read[read.length-1].humdity;
        read.forEach((val, index)=>{
            val._id = null;//delete val._id;
            val.humdity = null;
        })
        console.log(led);   
        res.render("first",{parameter : read, hum: something, led: led}); 
    });
});

router.get("/test/",(req,res)=>{
    res.render("first");
});

router.post("/data/rcv/nodemcu",(req,res)=>{
    let new_time = new Date();
    //const new_min = new_time.getMinutes();
    let new_hour = new_time.getHours() + (new_time.getMinutes()/60) + (5.30);
    //let new_date = new_time.getDate();
    //dataCollect((data)=>{    
        let newData = {
            y : req.body.temp,
            humdity : req.body.humdity,
            weather : "Mostly Sunny",//data.response.ob.weather,
            x : new_hour,//.toFixed(5),
            //s_date : new_date
        };
        if (req.body.temp != 'nan'){
            Data.create(newData, (err, data)=>{
                if (err) throw err;
                const x_num = String(led);
                res.send(x_num);
            });
        }else{
            const x_num = String(led);
            res.send(x_num);
        } 
        //});
});

router.get("/led/green/",(req,res)=>{
    led = 1;
    res.send("OK");
});

router.get("/led/red",(req,res)=>{
    led = 0;
    res.send("OK");
});

module.exports = router;
