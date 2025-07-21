#include <stdio.h>
#include <stdlib.h>

int main() {
    int choice;
    
    printf("You enter a dark and moist forest, you hear a frightening rumbling similar to a \"uuuuuurrraaaaa\" in the distance and a faint stomp sound. There are 2 paths:\n");
    printf("1. Darker forest (there seems to be a card on the ground)\n");
    printf("2. A free field with grass.\n");
    printf("Enter your choice (1 or 2): ");
    scanf("%d", &choice);
    
    if (choice == 1) {
        printf("\nThe rumbling is still persistent, yet you feel determined to get back home, you see the light through the trees when you look up, when you look down you see a shiny card with some writing on it, pick it up?\n");
        printf("1. yes\n");
        printf("2. no\n");
        printf("Enter your choice (1 or 2): ");
        scanf("%d", &choice);
        
        if (choice == 1) {
            printf("\nYou picked up Usagi's level 3 weeding licence, it was a trap...\n");
            printf("you died\n");
        } else if (choice == 2) {
            printf("\nYou were smart and didn't touch randoms on the ground, your gf comes and saves u on a helicopter, and gives you KFC Dubai zinger burger. Marry her?\n");
            printf("1. yes\n");
            printf("2. no\n");
            printf("Enter your choice (1 or 2): ");
            scanf("%d", &choice);
            
            if (choice == 1) {
                printf("\nYou survived and live happily ever after in a mansion in Saskatchewan after selling your souls to capitalism, you adopt a cat and name it Big red's Dougherty Dozen back acne\n");
            } else if (choice == 2) {
                printf("\nYou die instantly and get transported to that layer of hell where you are on a platform in the middle of a pirhana infested water and your only options are to jump in the water or be infinitely tortured by a rotating rock on the platform (Ik you've seen that yt short)\n");
            } else {
                printf("\nInvalid choice. Game over.\n");
            }
        } else {
            printf("\nInvalid choice. Game over.\n");
        }
    } else if (choice == 2) {
        printf("\nYou hear UNA YAHA YAHA UNA behind you... you die instantly\n");
    } else {
        printf("\nInvalid choice. Game over.\n");
    }
    
    return 0;
}