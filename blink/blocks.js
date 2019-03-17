
var mqtt_color_hue = 92;

Blockly.Blocks["mqtt.connect"] = {
	init: function() {
		this.appendDummyInput()
			.appendField("Connect mqtt server");

		this.appendValueInput('IP')
			.setCheck('String')
			.appendField("IP");

		this.setInputsInline(false);
		this.setPreviousStatement(true);
		this.setNextStatement(true);
		this.setColour(mqtt_color_hue);
		this.setTooltip(Blockly.Msg.BLINK_START_TOOLTIP);
		this.setHelpUrl(Blockly.Msg.BLINK_START_HELPURL);
	}
};

Blockly.Blocks["mqtt.pub"] = {
	init: function() {
		this.appendDummyInput()
			.appendField("Publish mqtt");

		this.appendValueInput('TOPIC')
			.setCheck('String')
			.appendField("Topic");

		this.appendValueInput('DATA')
			.setCheck('Number')
			.appendField("Data");

		this.setInputsInline(false);
		this.setPreviousStatement(true);
		this.setNextStatement(true);
		this.setColour(mqtt_color_hue);
		this.setTooltip(Blockly.Msg.BLINK_START_TOOLTIP);
		this.setHelpUrl(Blockly.Msg.BLINK_START_HELPURL);
	}
};

Blockly.Blocks["mqtt.sub"] = {
	init: function() {
		this.appendDummyInput()
			.appendField("Subscribe mqtt");

		this.appendValueInput('TOPIC')
			.setCheck('String')
			.appendField("Topic");

		this.appendStatementInput("DO")
			.appendField("do");

		this.setInputsInline(false);
		this.setPreviousStatement(true);
		this.setNextStatement(true);
		this.setColour(mqtt_color_hue);
		this.setTooltip(Blockly.Msg.BLINK_START_TOOLTIP);
		this.setHelpUrl(Blockly.Msg.BLINK_START_HELPURL);
	}
};

Blockly.Blocks["mqtt.is_connected"] = {
	init: function() {
		this.appendDummyInput()
			.appendField("Is connect mqtt");

		this.setOutput(true, 'Boolean');
		this.setInputsInline(false);
		this.setColour(mqtt_color_hue);
		this.setTooltip(Blockly.Msg.BLINK_START_TOOLTIP);
		this.setHelpUrl(Blockly.Msg.BLINK_START_HELPURL);
	}
};

Blockly.Blocks["mqtt.subscribe_data"] = {
	init: function() {
		this.appendDummyInput()
			.appendField("data");

		this.appendValueInput('TOPIC')
			.setCheck('String')
			.appendField("Topic");

		this.setOutput(true, 'String');
		this.setInputsInline(false);
		this.setColour(mqtt_color_hue);
		this.setTooltip(Blockly.Msg.BLINK_START_TOOLTIP);
		this.setHelpUrl(Blockly.Msg.BLINK_START_HELPURL);
	}
};