#include "sched.h"
	/*
	 * Boletos por prioridad
	 * prioridad 1 -> 50 boletos
	 * prioridad 2 -> 25 boletos
	 * prioridad 3 -> 10 boletos
	 * prioridad 4 -> 5 boletos
	 */
int lottery_priorities[4]={50,25,10,5};
int lott_quantum=3;
struct lottery_data{
	int boletos;
	int remaining_ticks;
};


static int task_new_lott(task_t* t)
{
	struct lottery_data* cs_data=malloc(sizeof(struct lottery_data));

	if (!cs_data)
		return 1;  /* Cannot reserve memory */
	cs_data->boletos = lottery_priorities[t->prio];
	cs_data->remaining_ticks = lott_quantum;

	t->tcs_data=cs_data;
	return 0;
}

static task_t* pick_next_task_lott(runqueue_t* rq)
{
	srand(time(NULL));
	struct lottery_data* cs_data=malloc(sizeof(struct lottery_data));
	task_t* it=head_slist(&rq->tasks);
	int totalTickets = 0;
	while(it){
		cs_data = (struct lottery_data*) it->tcs_data;
		totalTickets += cs_data->boletos;
		it = next_slist(&rq->tasks,it);
	}
	if(totalTickets){
	int afortunado = rand()%totalTickets;
	it=head_slist(&rq->tasks);
	cs_data = (struct lottery_data*) it->tcs_data;
	int fin = 0;
	//un Flag de parada
	int proceed = 1;
	while(proceed&&it){
		fin +=cs_data->boletos;
		if(afortunado<fin){
			proceed = 0;
		}else{
			it = next_slist(&rq->tasks,it);
			cs_data = (struct lottery_data*) it->tcs_data;
		}
	}
	cs_data->boletos--;
	cs_data->remaining_ticks = lott_quantum;
	}

	/* Current is not on the rq -> let's remove it */
	if(it){
		it->tcs_data = cs_data;
		remove_slist(&rq->tasks,it);
	}

	return it;
}

static void enqueue_task_lott(task_t* t,runqueue_t* rq, int preempted)
{

	if (t->on_rq || is_idle_task(t))
		return;

	insert_slist(&rq->tasks,t); //Push task
	//cs_data->remaining_ticks_slice=rr_quantum; // Reset slice
}
static void task_tick_lott(runqueue_t* rq)
{
	task_t* current=rq->cur_task;
	struct lottery_data* cs_data=(struct lottery_data*) current->tcs_data;

	if (is_idle_task(current))
		return;

	cs_data->remaining_ticks--; /* Charge tick */

	if (cs_data->remaining_ticks<=0)
		rq->need_resched=TRUE; //Force a resched !!
}


static task_t* steal_task_lott(runqueue_t* rq)
{
	task_t* t=tail_slist(&rq->tasks);

	if (t)
		remove_slist(&rq->tasks,t);

	return t;
}

static void task_free_lott(task_t* t)
{
	if (t->tcs_data) {
		free(t->tcs_data);
		t->tcs_data=NULL;
	}
}

sched_class_t lott_sched= {
	.task_new=task_new_lott,
	.pick_next_task=pick_next_task_lott,
	.enqueue_task=enqueue_task_lott,
	.task_tick=task_tick_lott,
	.steal_task=steal_task_lott,
	.task_free=task_free_lott,
};
