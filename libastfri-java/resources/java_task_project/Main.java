import java.util.List;

public class Main {
    public static void main(String[] args) {
        TaskRepository repo = new TaskRepository();

        repo.add(new Task(1, "Dokonči projekt", "in_progress"));
        repo.add(new Task(2, "Napiš dokumentáciu", "todo"));
        repo.add(new Task(3, "Odošli e-mail", "done"));
        repo.add(new Task(4, "Urob prezentáciu", "todo"));

        TaskFilter todoFilter = new StatusFilter("todo");
        List<Task> todoTasks = repo.filter(todoFilter);

        for (int i = 0; i < todoTasks.size(); i++) {
            System.out.println(todoTasks.get(i));
        }
    }
}
