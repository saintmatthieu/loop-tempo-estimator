import os

def apply_style(plt):
    for spine in plt.gca().spines.values():
        # Set to white
        spine.set_color("white")
    # set tick and tick label also to white, and make them bigger
    plt.tick_params(axis='x', colors='white', labelsize=10)
    plt.tick_params(axis='y', colors='white', labelsize=10)
    plt.gca().set_facecolor('black')
    plt.gcf().set_facecolor('black')
    # get x and y labels and set the font to white
    plt.xlabel(plt.gca().get_xlabel(), color='white')
    plt.ylabel(plt.gca().get_ylabel(), color='white')
    # also do that for the title
    plt.title(plt.gca().get_title(), color='white')


def save_figure(plt, filename):
    plt.savefig(os.path.join(os.path.dirname(
        __file__), filename), transparent=True)
