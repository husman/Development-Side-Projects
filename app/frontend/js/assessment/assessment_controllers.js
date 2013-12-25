// Controllers
angular.module('AssessmentControllers', []).
controller('AssessmentController', 
	function($scope, AssessmentFactory) {
		// Assessment Model retrieval/manipulation methods.
		// Assessment info (Read-Only, Data retrieval)
		$scope.sections = AssessmentFactory.getSections();
		$scope.info = AssessmentFactory.getInfo();
		
		// Assessment info (Write-Only, Data update)
		$scope.setTitle = function(newValue) {
			// Do any additional filtering/manipulation here.
			AssessmentFactory.setTitle(newValue);
		};
		$scope.setType = function(newValue) {
			// Do any additional filtering/manipulation here.
			AssessmentFactory.setType(newValue);
		};
		$scope.setTimeLimit = function(newValue) {
			// Do any additional filtering/manipulation here.
			AssessmentFactory.setTimeLimit(newValue);
		};
		$scope.setStartDate = function(newValue) {
			// Do any additional filtering/manipulation here.
			AssessmentFactory.setStartDate(newValue);
		};
		$scope.setEndDate = function(newValue) {
			// Do any additional filtering/manipulation here.
			AssessmentFactory.setEndDate(newValue);
		};
		$scope.createSection = function(type) {
			// Prepare the data and do any additional tasks here.
			var sectionInfo = {
					title: '',
					instructions: '',
					type: type,
					questionTypeClass: function() {
						switch(type) {
						case 'Multiple Choice':
							return 'multiple-choice';
							break;
						case 'Fill in the Blanks':
							return 'fill-in-blanks';
							break;
						case 'Matching':
							return 'matching';
							break;
						}
					}()
			};
			AssessmentFactory.addSection(sectionInfo);
		};
		
		// Additional Read-Only properties related to the Model
		$scope.getNumQuestions = function() {
			return AssessmentFactory.getNumQuestions();
		};
		$scope.getTotalPoints = function() { 
			return AssessmentFactory.getTotalPoints();
		};
		
		// Additional properties not related to the Model
		// but necessary for an Assessment's functionality.
		$scope.editMode = false;
		
		// Additional Util methods not related to the Model
		// but necessary for an Assessment's functionality. 
		$scope.compare = function(a,b) {
			return (angular.equals(a,b)) ? true : false;
		};
		
		$scope.logFactory = function() {
			console.log(AssessmentFactory.printToConsole());
		};
	}
);