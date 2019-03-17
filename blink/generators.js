Blockly.JavaScript['wifi.start'] = function(block) {
	var ssid = Blockly.JavaScript.valueToCode(block, 'SSID', Blockly.JavaScript.ORDER_ASSIGNMENT) || '0';
	var password = Blockly.JavaScript.valueToCode(block, 'PASSWORD', Blockly.JavaScript.ORDER_ASSIGNMENT) || '0';
	return 'DEV_IO.BLINK().connect_wifi('+ssid+','+password+');\n';
};

Blockly.JavaScript['wifi.ssid'] = function(block) {
	return ['DEV_IO.BLINK().ssid()'];
};

Blockly.JavaScript['wifi.password'] = function(block) {
	return ['DEV_IO.BLINK().password()'];
};

Blockly.JavaScript['mqtt.connect'] = function(block) {
	var ip = Blockly.JavaScript.valueToCode(block, 'IP', Blockly.JavaScript.ORDER_ASSIGNMENT) || '(char *)"mqtt://broker.mqttdashboard.com"';
	return 'DEV_IO.BLINK().mqtt_start('+ip+');\n';
};

Blockly.JavaScript['mqtt.pub'] = function(block) {
	var topic = Blockly.JavaScript.valueToCode(block, 'TOPIC', Blockly.JavaScript.ORDER_ASSIGNMENT) || '(char *)"/topic/qos0"';
	var data = Blockly.JavaScript.valueToCode(block, 'DATA', Blockly.JavaScript.ORDER_ASSIGNMENT) || '0';
	console.log(data);
	return 'DEV_IO.BLINK().mqtt_pub('+topic+','+data+');\n';
};

Blockly.JavaScript['mqtt.sub'] = function(block) {
	// return '';
	var topic = Blockly.JavaScript.valueToCode(block, 'TOPIC', Blockly.JavaScript.ORDER_ASSIGNMENT) || '(char *)"/topic/qos0"';
	var statement = Blockly.JavaScript.statementToCode(block, 'DO');
	return 'DEV_IO.BLINK().mqtt_sub('+topic+',[](){'+statement+'});\n';
};

Blockly.JavaScript['mqtt.is_connected'] = function(block) {
	return ['DEV_IO.BLINK().mqtt_is_connected()'];
};

Blockly.JavaScript['mqtt.subscribe_data'] = function(block) {
	var topic = Blockly.JavaScript.valueToCode(block, 'TOPIC', Blockly.JavaScript.ORDER_ASSIGNMENT) || '(char *)"/topic/qos0"';
	return ['DEV_IO.BLINK().subscribe_data('+topic+')'];
};

// Blockly.JavaScript[.BLINK.stop'] = function(block) {
// 	return 'DEV_IO.BLINK().stop();\n';
// };
