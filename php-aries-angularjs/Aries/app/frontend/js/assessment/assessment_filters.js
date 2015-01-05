// Filters
angular.module('AssessmentFilters', []).
filter('timeLimitFormat', function() {
	return function(timeLimitHours, timeLimitMinutes) {
		return (timeLimitHours > 0? timeLimitHours + " Hour" + (timeLimitHours > 1? "s": "") : "")
			   + (timeLimitHours > 0 && timeLimitMinutes > 0? "," : "")
			   + (timeLimitMinutes > 0? " " + timeLimitMinutes + " Minute" + (timeLimitMinutes > 1? "s": "") : "");
	};
}).
filter('dateRangeFormat', function() {
	return function(startDate, endDate) {
		if(startDate === "" || endDate === "") {
			return "";
		}
		return startDate + ' to ' + endDate;
	};
}).
filter('fillInBlankText', function() {
	return function(value, isBlank, options, appendDelimiter) {
		var delimiter = appendDelimiter? options.delimiter : '';
		
		if(delimiter === 'Space') {
			delimiter = ' ';
		} else if(delimiter === 'Custom') {
			delimiter = options.customDelimiter;
		}
		
		if(isBlank) {
			return "___" + delimiter;
		}
		
		return value + delimiter;
	};
});