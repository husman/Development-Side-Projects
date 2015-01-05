// Directives
angular.module('AssessmentDirectives', []).
directive('section', function() {
	return {
		restrict: 'A',
		templateUrl: '/index.php?controller=assessment&action=templates&param=section',
		scope: {
			sectionData: "=section",
			sectionClass: "=last"
		},
		controller: 'AssessmentController',
		link: function ($scope, $elem, $attrs) {
			console.log($attrs);
			
			// Ready-Only methods related to Model
			$scope.getSectionTotalPoints = function() {
				var numQuestions = $scope.sectionData.questions.length,
					totalPoints = 0;
				for(var i=0; i<numQuestions; ++i) {
					totalPoints += parseFloat($scope.sectionData.questions[i].totalPoints);
				}
				return totalPoints;
			};
			
			$scope.getSectionTotalQuestions = function() {
				return $scope.sectionData.questions.length;
			};
			
			// Util methods related to Model
			$scope.addQuestion = function(type) {
				var questionData = {
						type: '', 
	   	            	num: $scope.sectionData.questions.length, 
	   	            	totalPoints: 0,
	   	            	question: '',
	   	            	correctAns: {
	   	            		num: 0,
	   	            		description: ''
	   	            	},
	   	            	choices: [],
	   	            	options: null
				};
				switch(type) {
				case 'Multiple Choice':
					questionData.type = 'multipleChoice';
					questionData.options = {
							randomize: 'Yes'	
					};
					questionData.choices.push(
							{
								num: 0,
		            	        description: ''
		            	    },
		            	    {
		            	    	num: 1,
		            	        description: ''
		            	    }
		            );
					$scope.sectionData.questions.push(questionData);
					break;
				case 'Fill in the Blanks':
					questionData.type = 'fillInTheBlanks';
					questionData.options = {
							delimiter: 'Space',
							customDelimiter: '',
							showWordBank: 'Yes'
					};
					$scope.sectionData.questions.push(questionData);
					break;
				case 'Matching':
					questionData.type = 'matching';
					questionData.instructions = '';
					questionData.options = {
							randomize: 'Yes'	
					};
					questionData.choices.push(
							{
								num: 0,
		            	        statement: '',
		            	        answer: '',
		            	        matchingType: 'StatementAndAnswer'
		            	    },
		            	    {
		            	    	num: 1,
		            	    	statement: '',
		            	        answer: '',
		            	        matchingType: 'StatementAndAnswer'
		            	    },
		            	    {
								num: 2,
		            	        statement: '',
		            	        answer: '',
		            	        matchingType: 'StatementAndAnswer'
		            	    },
		            	    {
		            	    	num: 3,
		            	    	statement: '',
		            	        answer: '',
		            	        matchingType: 'StatementAndAnswer'
		            	    }
		            );
					$scope.sectionData.questions.push(questionData);
					console.log($scope.sectionData.questions);
					break;
				}
			};
			
		}
	};
}).
directive('multipleChoice', function() {
	return {
		restrict: 'A',
		templateUrl: '/index.php?controller=assessment&action=templates&param=multipleChoice',
		scope: {
			questionData: "=multipleChoice",
			questionClass: "=last"
		},
		controller: 'AssessmentController',
		link: function ($scope, $elem, $attrs) {
			// Util methods related to Model
			$scope.addAnswerChoice = function() {
				console.log($scope.questionData.choices);
				$scope.questionData.choices.push(
						{
							num: $scope.questionData.choices.length,
							description: ''
						}
				);
			};
			console.log("MULTIPLE CHOICE");
			console.log($attrs);
		}
	};
}).
directive('fillInTheBlanks', function() {
	return {
		restrict: 'A',
		templateUrl: '/index.php?controller=assessment&action=templates&param=fillInTheBlanks',
		scope: {
			questionData: "=fillInTheBlanks",
			questionClass: "=last"
		},
		controller: 'AssessmentController',
		link: function ($scope, $elem, $attrs) {
			// Util methods related to Model
			$scope.makeAnswers = function() { // DEPRECATED (WILL REMOVE IN FUTURE)
				var textElement = $($elem).find('.crumsTextfield'),
					crumtext = $(textElement).val(),
					delimiter = $scope.questionData.options.delimiter;
				
				if(delimiter === 'Space') {
					delimiter = ' ';
				} else if(delimiter === 'Custom') {
					delimiter = $scope.questionData.options.customDelimiter;
				}
				
				if($.trim(crumtext) === '' || 
				   $.trim(crumtext) === delimiter) {
					return;
				}
				
				var btnAdd = $($elem).find('.addBtn'),
					delimiterOptions = $($elem).find('.delimiterOptions'),
					textvals = crumtext.split(delimiter),
					numCrums = textvals.length,
					choicesArray = [];
				
				if(numCrums < 1) {
					return;
				}
				
				// Hide text field/options and 'Add' button
				$(delimiterOptions).hide();
				$(textElement).hide();
				$(btnAdd).hide();
				
				for(var i=0; i<numCrums; ++i) {
					if($.trim(textvals[i]) !== '') {
						choicesArray.push({
							num: i,
							description: textvals[i],
							isBlank: false
						});
					}
				}
				
				$scope.questionData.choices = choicesArray;			
				$scope.questionData.correctAns = {
						num: 0,
   	            		description: crumtext
   	            };
				
				console.log($scope.questionData);
				
			};
			
			$scope.updateQuestionSpecs = function() {
				var textElement = $($elem).find('.crumsTextfield'),
					crumtext = $(textElement).val(),
					delimiter = '__';
				
				var textvals = crumtext.split(delimiter),
					crumSearch = crumtext.match(/__/g),
					numCrums = crumSearch? crumSearch.length : 0,
					choicesArray = [],
					currentChoicesLen = $scope.questionData.choices.length;
				
				
				// Load all old fields into array
				for(var i=0; i<numCrums; ++i) {
					if($.trim(textvals[i]) != '') {
						choicesArray.push({
							num: i,
							description: (currentChoicesLen - 1 >= i?
										  $scope.questionData.choices[i].description : ''),
							isBlank: false
						});
					}
				}
				
				$scope.questionData.choices = choicesArray;			
				$scope.questionData.correctAns = {
						num: 0,
		            	description: crumtext,
		            	humanDescription: crumtext
		        };
				console.log($scope.questionData);
			};
			console.log("Fill in the Blanks");
			console.log($attrs);
		}
	};
}).
directive('matching', function() {
	return {
		restrict: 'A',
		templateUrl: '/index.php?controller=assessment&action=templates&param=matching',
		scope: {
			questionData: "=matching",
			questionClass: "=last"
		},
		controller: 'AssessmentController',
		link: function ($scope, $elem, $attrs) {
			// Util methods related to Model
			$scope.addStatementAndAnswer = function() {
				$scope.questionData.choices.push(
						{
							num: $scope.questionData.choices.length,
							statement: '',
							answer: '',
							matchingType: 'StatementAndAnswer'
						}
				);
			};
			$scope.addAnswerOnly = function() {
				$scope.questionData.choices.push(
						{
							num: $scope.questionData.choices.length,
							answer: '',
							matchingType: 'AnswerOnly'
						}
				);
			};
			console.log("MATCHING");
		}
	};
});