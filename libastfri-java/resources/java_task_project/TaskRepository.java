import java.util.ArrayList;
import java.util.List;

public class TaskRepository {
    private final List<Task> tasks = new ArrayList<>();

    public void add(Task task) {
        tasks.add(task);
    }

    public List<Task> getAll() {
        return tasks;
    }

    public List<Task> filter(TaskFilter filter) {
        List<Task> result = new ArrayList<>();
        for (int i = 0; i < tasks.size(); i++) {
            Task task = tasks.get(i);
            if (filter.appliesTo(task)) {
                result.add(task);
            }
        }
        return result;
    }
}
