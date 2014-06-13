// Factories
angular.module('AssessmentFactories', []).
factory('AssessmentFactory', function() {
	var _info = {
			title: "",
			type: "Quiz/Test",
			timeLimitHours: 0,
			timeLimitMinutes: 0,
			startDate: "",
			endDate: ""
		},
		_sections = [];
	
	return {
		// Member getters
		getTitle : function() {
			return _info.title;
		},
		getType : function() {
			return _info.type;
		},
		getTimeLimit : function() {
			return _info.timeLimit;
		},
		getStartDate : function() {
			return _info.startDate;
		},
		getEndDate : function() {
			return _info.endDate;
		},
		getSections : function() {
			return _sections;
		},
		getInfo : function() {
			return _info;
		},
		
		// Member setters
		setTitle : function(value) {
			_info.title = value;
		},
		setType : function(value) {
			_info.type = value;
		},
		setTimeLimit : function(value) {
			_info.timeLimit = value;
		},
		setStartDate : function(value) {
			_info.startDate = value;
		},
		setEndDate : function(value) {
			_info.endDate = value;
		},
		
		// Util methods for Model
		getNumQuestions : function() {
			var numSections = _sections.length,
				totalNumQuestions = 0;
			for(var i=0; i<numSections; ++i) {
				totalNumQuestions += _sections[i].questions.length;
			}
			return totalNumQuestions;
		},
		getTotalPoints : function() {
			var numSections = _sections.length,
				totalPoints = 0;
			for(var i=0; i<numSections; ++i) {
				var numQuestions = _sections[i].questions.length;
				for(var z=0; z<numQuestions; ++z) {
					totalPoints += parseFloat(_sections[i].questions[z].totalPoints);
				}
			}
			return totalPoints;
		},
		addSection : function(sectionInfo) {
			_sections.push({
				// Model related properties
	           	 title: sectionInfo.title,
	           	 description: sectionInfo.instructions,
	           	 type: sectionInfo.type,
	           	 questionTypeClass: sectionInfo.questionTypeClass,
	           	 questions: [],
	           	 
	           	 // Other essential properties
	           	 num: _sections.length,
	           	 collapsed: true,
	           	 editMode: false
			});
		},
		printToConsole : function() {
			console.log(_info);
			console.log(_sections);
		}
	};
});