/*jslint node: true */
"use strict";

var createModule = require("../module").createModule;
var createInstaller = require("../installer/installer");
var config = require("../config");

var createEventData = function(eventID, progressData) {
	// change the progress into events to be consistent with ds
	var data = {
		name: "Installing Target Support ( First Time Only )",
		subActivity: progressData.message,
		id: eventID,
		isComplete: progressData.isComplete ? true : false,
		isFirstUpdate: progressData.isFirstUpdate ? true : false,
		percent: undefined !== progressData.percent ? progressData.percent : null
	};

	return data;
};

module.exports = {

	name: "TargetSupport",

	create: function() {

		// get cloudAgentInstallerServerURL from the config file set by the agent.setProperty call 
		// by the client
		var installer = createInstaller(config.cloudAgentInstallerServerURL);

		var eventID = 0;

		return createModule(this.name).then(

			function(targetSupport) {

				targetSupport.commands.add = function(ccxmlFilePath, disableG2Override) {

					eventID++;

					return installer.addSupport(ccxmlFilePath, disableG2Override)
						.progress(function(progressData) {
							targetSupport.triggerEvent("progress", createEventData(eventID, progressData));
						});
				};

				targetSupport.commands.purge = function() {
					return installer.purge();
				};

				return {
					port: targetSupport.getPort()
				};

			}

		);

	}

};