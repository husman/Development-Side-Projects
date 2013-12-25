<!--  Next Source Inc Demostration by Haleeq Usman -->
<!DOCTYPE html>
<html>
<head>
<meta http-equiv="X-UA-Compatible" content="IE=edge,chrome=1">
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<title>NextSource Demostration</title>

<!-- Front-end External CSS Deps -->
<link type="text/css" rel="stylesheet" href="http://fonts.googleapis.com/css?family=Open+Sans:400,600,700" />
<link type="text/css" rel="stylesheet" href="/app/frontend/css/bootstrap.css" />

<!-- Front-end CSS -->
<link type="text/css" rel="stylesheet" href="/app/frontend/css/core.css" />
<link type="text/css" rel="stylesheet" href="/app/frontend/css/assessment.css" />

<!-- Front-end Dep JS Libraries -->
<script type="text/javascript" src="/app/frontend/js/jquery-1.9.1.min.js"></script>
<script type="text/javascript" src="/app/frontend/js/bootstrap.min.js"></script>

<!-- Front-end MVVC Framework -->
<script type="text/javascript" src="/app/frontend/js/angular.min.js"></script>

<!-- Modularized MVC application -->
<script type="text/javascript" src="/app/frontend/js/assessment/assessment_factories.js"></script>
<script type="text/javascript" src="/app/frontend/js/assessment/assessment_controllers.js"></script>
<script type="text/javascript" src="/app/frontend/js/assessment/assessment_directives.js"></script>
<script type="text/javascript" src="/app/frontend/js/assessment/assessment_filters.js"></script>
<script type="text/javascript" src="/app/frontend/js/assessment/assessment_module.js"></script>

</head>
<body>
	<div class="wrap">
		<div class="innerWrap">
			<div class="mainContent">
				<div id="body">
					<div ng-app="AssessmentModule">
						<div ng-controller="AssessmentController">
							<div class="pageTitle">Create An Assessment</div>
							<br />
							<div class="assignmentType">NextSource Demostration</div>
							<br />
							<div class="pageDesc">
								<div style="font-weight: bold; color:#2f96b4; padding-bottom:10px;
											text-decoration:underline; font-size:18px;">
									Author: Haleeq Usman
								</div>
								This is a demostration I have wrote up for David @ Next Source. 
								The back-end is served by a <span class="red bold">very light</span>  
								custom <span class="red bold">MVC</span> framework I wrote up within a 
								few hours. Because this is a demostration, I have attempted to convey as
								much detail about my coding style and general thinking. Therefore, the 
								framework is very light weight but it touches on some important features 
								including: <span class="red bold">Autoloading</span> (utilized a Registry 
								method from Codeigniter - A design pattern we discussed), 
								<span class="red bold">Routing</span>, 
								<span class="red bold">Database abstraction</span> (MySQL and Oracle 11 XE) ,
								<span class="red bold">Controllers</span>, 
								<span class="red bold">Views</span>, and 
								<span class="red bold">Configuration</span> for different 
								<span class="red bold">Development Environments</span>.
								<br /><br />
								I like to separate the <span class="red bold">layers of my application</span> 
								as much as possible and break the components of my application/software into 
								<span class="red bold">front-end</span> and 
								<span class="red bold">back-end</span> components. 
								The front-end <span class="red bold">(HTML, JavaScript, and CSS)</span> has 
								follows the same <span class="red bold">Design Pattern</span> as the back-end 
								<span class="red bold">(PHP)</span>. The front-end achieves MVC through the help 
								of <span class="red bold">AngularJS</span>. Though I have not done anythng with 
								the front-end data to persist it with the back-end for this demostration.
							</div>
							<br />
							<div class="createTitleBox">
								<div class="sectionTitle left">Assessment Info</div>
								<a class="removeSectionBtn right" href="javascript:void(0);"
									style="margin: 0px 6px;"> <img
									src="/app/frontend/images/buttons/trashBtn.png" alt="Delete"
									width="28" height="27" />
								</a> <a class="editSectionBtn right" href="javascript:void(0);"
									style="margin: 0px 6px;" data-ng-class="{active: editMode}"
									data-ng-click="editMode = true">&#32;</a>
								<div class="clear"></div>
								<br /> <br />
								<div data-ng-show="!editMode">
									<table cellspacing="5" cellpading="5" class="assessmentTable">
										<tr>
											<td class="titleLabel">Test Title</td>
											<td data-ng-bind="info.title"></td>
										</tr>
										<tr>
											<td class="titleLabel">Test Type</td>
											<td data-ng-bind="info.type"></td>
										</tr>
										<tr>
											<td class="titleLabel">Total Questions</td>
											<td data-ng-bind="getNumQuestions() | number"></td>
										</tr>
										<tr>
											<td class="titleLabel">Total Points</td>
											<td data-ng-bind="getTotalPoints() | number"></td>
										</tr>
										<tr>
											<td class="titleLabel">Time Limit</td>
											<td
												data-ng-bind="info.timeLimitHours | timeLimitFormat: info.timeLimitMinutes"></td>
										</tr>
										<tr>
											<td class="titleLabel">Active Test Period</td>
											<td
												data-ng-bind="info.startDate | dateRangeFormat: info.endDate"></td>
										</tr>
									</table>
								</div>

								<div data-ng-show="editMode">
									<table cellspacing="5" cellpading="5" class="assessmentTable">
										<tr>
											<td class="titleLabel">Test Title</td>
											<td><input type="text" class="widefield"
												data-ng-model="info.title" data-ng-click="logFactory()" /></td>
										</tr>
										<tr>
											<td class="titleLabel">Test Type</td>
											<td><select id="assessment_type">
													<option value="Exam/Quiz">Quiz/Test</option>
													<option value="Survey">Survey</option>
											</select></td>
										</tr>
										<tr>
											<td class="titleLabel">Total Questions</td>
											<td data-ng-bind="getNumQuestions() | number"></td>
										</tr>
										<tr>
											<td class="titleLabel">Total Points</td>
											<td data-ng-bind="getTotalPoints() | number"></td>
										</tr>
										<tr>
											<td class="titleLabel">Time Limit</td>
											<td>
												<div class="left timelimit_hrs">
													<input type="text" class="smallfield left"
														data-ng-model="info.timeLimitHours" /> <span
														class="label left">Hours</span>
													<div class="clear"></div>
												</div>
												<div class="left timelimit_mins">
													<input type="text" class="smallfield left"
														data-ng-model="info.timeLimitMinutes" /> <span
														class="label left">Minutes</span>
													<div class="clear"></div>
												</div>
												<div class="clear"></div>
											</td>
										</tr>
										<tr>
											<td class="titleLabel">Active Test Period</td>
											<td>
												<div class="dateRange">
													<div class="left normalfield">
														<input type="text" id="startDate" name="startDate"
															required
															data-msg-required="Please the start date of the assignment"
															data-ng-model="info.startDate" />
													</div>
													<div class="left daterange_to">to</div>
													<div class="left normalfield">
														<input type="text" id="endDate" name="endDate" required
															data-msg-required="Please the end date of the assignment"
															data-ng-model="info.endDate" />
													</div>
													<!--<div class="editorStatus right">last saved Today, 6:53 PM</div>-->
													<div class="clear"></div>
												</div>

											</td>
										</tr>
										<tr>
											<td class="titleLabel"></td>
											<td><br /> <a href="javaScript:void(0);" class="btn-done"
												data-ng-click="editMode = false">Save</a></td>
										</tr>
									</table>
								</div>
							</div>

							<br /> <br />
							<div class="accordion sections" id="accordion_main"></div>
							
							<div data-ng-repeat="section in sections">
								<div data-section="section" data-last="$last" class="section"></div>
							</div>
							<br />

							<div class="btn-group left addSectionQuestionBtn createSection">
								<button data-toggle="dropdown"
									class="btn btn-primary dropdown-toggle"></button>
								<ul class="dropdown-menu">
									<li><a href="javascript:void(0);" title="Multiple Choice"
										data-ng-click="createSection('Multiple Choice')">&gt; Multiple
											Choice</a></li>
									<li><a href="javascript:void(0);" title="Fill in the Blanks"
										data-ng-click="createSection('Fill in the Blanks')">&gt; Fill
											in the Blanks</a></li>
									<li><a href="javascript:void(0);" title="Matching"
										data-ng-click="createSection('Matching')">&gt; Matching</a></li>
								</ul>
							</div>
							<div class="clear"></div>
							<br /><br />
						</div>
					</div>
				</div>
			</div>
		</div>
	</div>
</body>
</html>