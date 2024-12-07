package emergency;

import java.io.*;
import java.util.*;

public enum PatientStatus {
    WAITING_FOR_NURSE,
    WITH_NURSE,
    WAITING_IN_ROOM,
    WITH_PROVIDER,
    WAITING_FOR_CHECKOUT,
    WITH_ADMIN,
    COMPLETED
}
