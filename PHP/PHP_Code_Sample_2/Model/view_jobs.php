<?php session_start(); 
require_once 'Job.php';

$jobs = Job::getAll(array("active" => true));
?>
<!DOCTYPE html>
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<title>BT - Jobs</title>
</head>
<body>
<h2>Welcome to our Job listings!</h2><br />

<h4>Here are all the open jobs:</h4>
<?php
if(!empty($jobs)) {
	if(!is_array($jobs)) {
		foreach($jobs as $job) { ?>
			<div class="job">
				<strong><?= $job->title; ?></strong><br /><br />
				<div class="subtext">
					<span class="department">Department: <?= $job->department; ?></span> | 
					<span class="onlocation">On Location: <?= $job->onlocation; ?></span>
				</div><br />
				<div class="description">
					<?= htmlspecialchars($job->description); ?>
				</div><br />
				<h5>Additional Requirements:</h5><a></a>
				<div class="additional_req">
					<?= htmlspecialchars($job->requirements); ?>
				</div><br />
				Please contact <span class="email"><?= $job->contact_email; ?></span> for more 
				information.
			</div>
			<hr />
		<?php }
	} else { ?>
			<div class="job">
				<strong><?= $job->title; ?></strong><br /><br />
				<div class="subtext">
					<span class="department">Department: <?= $jobs->department; ?></span> | 
					<span class="onlocation">On Location: <?= $jobs->onlocation; ?></span>
				</div><br />
				<div class="description">
					<?= htmlspecialchars($jobs->description); ?>
				</div><br />
				<h5>Additional Requirements:</h5><a></a>
				<div class="additional_req">
					<?= htmlspecialchars($jobs->requirements); ?>
				</div><br />
				Please contact <span class="email"><?= $jobs->contact_email; ?></span> for more 
				information.
			</div>
	<?php }
} else { ?>
	<span class="nojobs">There are no active jobs.</span>
<?php } ?>
</body>
</html>