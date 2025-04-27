public class StatusFilter implements TaskFilter {
    private final String desiredStatus;

    public StatusFilter(String status) {
        this.desiredStatus = status;
    }

    @Override
    public boolean appliesTo(Task task) {
        String rawStatus = task.getStatus();
        String normalizedStatus = rawStatus.toLowerCase();
        String normalizedDesired = desiredStatus.toLowerCase();
        boolean result = false;

        switch (normalizedStatus) {
            case "todo":
                result = normalizedStatus.equals(normalizedDesired);
                break;

            case "in_progress":
                result = normalizedStatus.equals(normalizedDesired);
                break;

            case "done":
                result = normalizedStatus.equals(normalizedDesired);
                break;

            default:
                System.out.println("Neznámy stav úlohy: " + rawStatus);
                break;
        }

        return result;
    }
}
