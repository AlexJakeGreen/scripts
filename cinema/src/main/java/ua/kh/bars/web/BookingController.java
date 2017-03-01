package ua.kh.bars.web;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.security.authentication.AnonymousAuthenticationToken;
import org.springframework.security.core.Authentication;
import org.springframework.security.core.context.SecurityContextHolder;
import org.springframework.security.core.userdetails.UserDetails;
import org.springframework.stereotype.Controller;
import org.springframework.transaction.annotation.Transactional;
import org.springframework.ui.Model;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;
import org.springframework.web.bind.annotation.RequestParam;
import org.springframework.web.bind.annotation.ResponseBody;
import ua.kh.bars.entities.Event;
import ua.kh.bars.entities.User;
import ua.kh.bars.services.BookingService;
import ua.kh.bars.services.EventService;
import ua.kh.bars.services.TicketService;
import ua.kh.bars.services.UserService;

/**
 * Created by green on 3/28/16.
 */

@Controller
@RequestMapping("/booking")
public class BookingController {

    @Autowired
    TicketService ticketService;

    @Autowired
    EventService eventService;

    @Autowired
    UserService userService;

    @Autowired
    BookingService bookingService;

    @RequestMapping(method = RequestMethod.POST)
    @Transactional(rollbackFor = Exception.class)
    public String bookTicket(@RequestParam("eventId") Long eventId,
                             @RequestParam("seatNumber") int seatNumber,
                             Model model) {
        User user = getCurrentUser();
        if (user != null) {
            Event event = eventService.findOne(eventId);
            if(null != bookingService.bookTicket(event, user, seatNumber)) {
                model.addAttribute("message", "Booked!");
            } else {
                model.addAttribute("message", "Booking failed!");
            }
            model.addAttribute("user", user);
            model.addAttribute("event", event);
        }
        return "events/show";
    }

    @RequestMapping(value = "getBalance", method = RequestMethod.GET, produces = "application/json")
    @ResponseBody
    public String getBalance() {
        User user = getCurrentUser();
        if (user != null) {
            int balance = user.getUserAccount().getMoneyPrepaid();
            return "{\"balance\": " + balance + "}";
        }
        return "{\"error\": \"Something went wrong\"}";
    }

    private User getCurrentUser() {
        Authentication authentication = SecurityContextHolder.getContext().getAuthentication();
        if (!(authentication instanceof AnonymousAuthenticationToken)) {
            UserDetails customUser = (UserDetails) authentication.getPrincipal();
            String userName = customUser.getUsername();
            return userService.findByUsername(userName);
        }
        return null;
    }

}
