struct io_t {
	/* Operacje modelu procesu */
	void (* wait_for_foot_switch) (struct io_t *);
	void (* pwm_set) (struct io_t *, int);
};

struct io_t * new_io(void);

