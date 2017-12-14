#include "sched.h"

	/*
	 * Boletos por prioridad
	 * prioridad 1 -> 50 boletos
	 * prioridad 2 -> 25 boletos
	 * prioridad 3 -> 10 boletos
	 * prioridad 4 -> 5 boletos
	 */


static int task_new_lott(task_t* t)
{
	struct rr_data* cs_data=malloc(sizeof(struct rr_data));

	if (!cs_data)
		return 1;  /* Cannot reserve memory */

	/* initialize the quantum */
	cs_data->remaining_ticks_slice=rr_quantum;
	t->tcs_data=cs_data;
	return 0;
}

static task_t* pick_next_task_lott(runqueue_t* rq)
{
	task_t* t=head_slist(&rq->tasks);

	/* Current is not on the rq -> let's remove it */
	if (t)
		remove_slist(&rq->tasks,t);

	return t;
}

static void enqueue_task_lott(task_t* t,runqueue_t* rq, int preempted)
{

	if (t->on_rq || is_idle_task(t))
		return;

	insert_slist(&rq->tasks,t); //Push task
	//cs_data->remaining_ticks_slice=rr_quantum; // Reset slice
}



static task_t* steal_task_lott(runqueue_t* rq)
{
	task_t* t=tail_slist(&rq->tasks);

	if (t)
		remove_slist(&rq->tasks,t);

	return t;
}


sched_class_t lott_sched= {
	.pick_next_task=pick_next_task_lott,
	.enqueue_task=enqueue_task_lott,
	.steal_task=steal_task_lott,
	.task_new=task_new_lott

};
