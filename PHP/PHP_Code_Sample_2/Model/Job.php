<?php

// This is an Example of a Job model.
class Job
{
   // Non-static members variables
   private $result_array;

   // We do not want them to change this
   // field for this simple design.
   private $id = null;
   public $title = null;
   public $description = null;
   public $requirements = null;
   public $contact_email = null;
   public $department = null;
   public $on_location = null;
   public $active = false;

   // If this does not link to the database
   // then it really does nothing other than
   // initializing the private member variables.
   function Job($job_id = NULL)
   {
      $this->$result_array = array();
      if ($job_id != NULL) {
         // follow the specifications and supply a where clause for query.
         $where_clause = "WHERE id=" . mysql_real_escape_string($job_id);

         // Select everything from the jobs table.
         $query = mysql_query("SELECT * FROM jobs $where_clause");

         // Clear result set. PHP's GC should take care of clearing
         // the heap of the previous object through reference count.
         $result_array = new Job();

         if ($row = mysql_fetch_object($query)) {
            // Sel values;
            $job->id = $row->id;
            $job->title = $row->title;
            $job->description = $row->description;
            $job->requirements = $row->requirements;
            $job->contact_email = $row->contact_email;
            $job->department = $row->department;
            $job->on_location = $row->on_location;
            $job->active = $row->active;

            $this->result_array = $job;
         }
      }

   public static function getAll()
   {
      // Select everything from the jobs table.
      $query = mysql_query("SELECT * FROM jobs WHERE 1");

      // HP's GC should take care of clearing the heap
      // of the previous object through reference count.
      $result_array = array();

      while ($row = mysql_fetch_object($query)) {
         $job = new Job();

         // Sel values;
         $job->id = $row->id;
         $job->title = $row->title;
         $job->description = $row->description;
         $job->requirements = $row->requirements;
         $job->contact_email = $row->contact_email;
         $job->department = $row->department;
         $job->on_location = $row->on_location;
         $job->active = $row->active;

         $result_array = $job;
      }

      return $result_array;
   }
	
  public static function getAll($where_array)
   {
      $where_clause = "WHERE 1";
      if (!empty($where_array)) {
         $where_clause = "WHERE ";
         foreach ($where_array as $column => $value) {
            $where_clause .= $column . "=" . "'" . mysql_real_escape_string($value) . "' AND";
         }
         // Remove last ' AND'
         substr($where_clause, 0, -4);
      }

      // Select everything from the jobs table.
      $query = mysql_query("SELECT * FROM jobs $where_clause");

      $result_array = array();

      while ($row = mysql_fetch_object($query)) {
         $job = new Job();

         // Sel values;
         $job->id = $row->id;
         $job->title = $row->title;
         $job->description = $row->description;
         $job->requirements = $row->requirements;
         $job->contact_email = $row->contact_email;
         $job->department = $row->department;
         $job->on_location = $row->on_location;
         $job->active = $row->active;

         $result_array[] = $job;
      }

      return $result_array;
   }
	
  public function save()
   {
      mysql_query("UPDATE jobs SET
					title='" . mysql_real_escape_string($this->title) . "'," .
         "description='" . mysql_real_escape_string($this->description) . "'," .
         "requirements='" . mysql_real_escape_string($this->requirements) . "'," .
         "contact_email='" . mysql_real_escape_string($this->contact_email) . "'," .
         "department='" . mysql_real_escape_string($this->department) . "'," .
         "on_location='" . mysql_real_escape_string($this->on_location) . "'," .
         "active='" . mysql_real_escape_string($this->active) . "'
					WHERE id='" . mysql_real_escape_string($this->id) . "'");
   }
	
  public function update_title($title)
   {
      $this->title = $title;

      // Save changes
      $this->save();
   }
	
	public function update_description(($desc) {
      $this->description = $desc;

      // Save changes
      $this->save();
   }
	
	public function update_requirements(($requirements) {
      $this->requirements = $requirements;

      // Save changes
      $this->save();
   }
	
	public function set_active()
   {
      $this->active = true;

      // Save changes
      $this->save();
   }
	
	public function set_inactive()
   {
      $this->active = false;

      // Save changes
      $this->save();
   }
}
?>